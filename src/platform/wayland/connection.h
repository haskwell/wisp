#pragma once
#include <sys/socket.h>
#include <sys/un.h>
#include <cstring>
#include <cstddef>
#include <unistd.h>
#include <cerrno>
#include <cstdlib>
#include <system_error>
#include <span>
#include <cstdint>
#include <vector>
#include <sys/uio.h>
#include <cstring>
#include <cstddef>
#include <utility>
#include "helper.h"

class Socket{
    int fd = -1;
    std::string path;

    public:
    Socket(){
        
        path = get_path();
        
        if(path.length() + 1 > sizeof(sockaddr_un::sun_path)){
            throw std::runtime_error("Path length too long");
        }

        fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
        if(fd < 0){
            throw std::system_error(errno, std::generic_category(),"Socket creation failed");
        }
    }

Socket(int existingFD) : fd(existingFD) {}

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    int getFD() const{
        return fd;
    }

    const std::string& getPath() const{
        return path;
    }

    ~Socket(){
        close(fd);
    }
};

class Connection{
    private:
        Socket socket;

    public:
Connection(int existingFD) : socket(existingFD) {}
    Connection(){
        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;

        strcpy(addr.sun_path, socket.getPath().c_str());
        
        socklen_t socklen = offsetof(struct sockaddr_un, sun_path) + socket.getPath().length() + 1;
        sockaddr* saddr = reinterpret_cast<sockaddr*>(&addr);
        
        if(connect(socket.getFD(), saddr, socklen) < 0){
            throw std::system_error(errno, std::generic_category(), "Connection failed");
        }
    }

    void sendMessage(const std::span<const uint8_t> message, const std::span<const int> fd){

        size_t requiredBytes = CMSG_SPACE(sizeof(int) * fd.size());
        std::vector<std::max_align_t> fdBuffer((requiredBytes + sizeof(std::max_align_t) - 1)/ sizeof(std::max_align_t));

        struct iovec iov{};
        iov.iov_base = (void*)message.data();
        iov.iov_len = message.size();

        struct msghdr msg{};

        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;

        if(!fd.empty()){
            msg.msg_control = fdBuffer.data();
            msg.msg_controllen = CMSG_SPACE(sizeof(int) * fd.size());

            cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
            cmsg->cmsg_level = SOL_SOCKET;
            cmsg->cmsg_type = SCM_RIGHTS;
            cmsg->cmsg_len = CMSG_LEN(sizeof(int) * fd.size());

            unsigned char* cmsgData = CMSG_DATA(cmsg);
            memcpy(cmsgData, fd.data(), fd.size_bytes());
        }

        size_t length = message.size();
        size_t sent = 0;
        bool fdSent = false;
        while(sent < length){
            ssize_t var = sendmsg(socket.getFD(), &msg, MSG_NOSIGNAL);
            if(var == -1 && errno == EINTR) continue;
            else if (var == -1){
                throw std::system_error(errno, std::generic_category(), "send");
            }
            else if(var == 0){
                throw std::runtime_error("connection unexpectedly closed");
            }
            else{
                if(!fdSent){
                    msg.msg_control = nullptr;
                    msg.msg_controllen = 0;
                    fdSent = true;
                }
                sent += var;
                iov.iov_base = (void*)(message.data() + sent);
                iov.iov_len -= var;
            }
        }
    }

    struct RecievedMessage{
        std::vector<uint8_t> message;
        std::vector<int> fds;
    };

    void processFDs(struct msghdr &msg, std::vector<int>& fds, size_t& fdsOldSize){
        if(msg.msg_flags & MSG_CTRUNC){
            throw std::runtime_error("not enough space for all FDs");
        }
        cmsghdr* cmsg = CMSG_FIRSTHDR(&msg);
        if(cmsg != nullptr && cmsg->cmsg_level == SOL_SOCKET
            && cmsg->cmsg_type == SCM_RIGHTS && cmsg->cmsg_len >= CMSG_LEN(0)){
            size_t cmsgLen = cmsg->cmsg_len - CMSG_LEN(0);
            unsigned char* cmsgData = CMSG_DATA(cmsg);
            if(cmsgLen % sizeof(int) != 0){
                throw std::runtime_error("corrupted stream");
            }
            int numOfFds = cmsgLen/sizeof(int);
            fds.resize(fds.size() + numOfFds);
            memcpy(fds.data() + fdsOldSize, cmsgData, cmsgLen);
            fdsOldSize = fds.size();
        }
    }

    RecievedMessage receiveMessage(){
        const size_t MAX_FDS = 16;
        std::vector<uint8_t> message(8);
        std::vector<int> fds;
        fds.reserve(MAX_FDS);
        struct iovec iov{};
        iov.iov_base = (void*)message.data();
        iov.iov_len = message.size();

        size_t requiredBytes = CMSG_SPACE(sizeof(int) * MAX_FDS);
        std::vector<std::max_align_t> fdBuffer((requiredBytes + sizeof(std::max_align_t) - 1)/ sizeof(std::max_align_t));

        struct msghdr msg{};
        msg.msg_iov = &iov;
        msg.msg_iovlen = 1;
        msg.msg_control = fdBuffer.data();
        msg.msg_controllen = requiredBytes;

        size_t headerLength = 8;
        size_t recieved = 0;
        size_t fdsOldSize = 0;
        while(recieved < headerLength){
            msg.msg_controllen = requiredBytes;
            ssize_t var = recvmsg(socket.getFD(), &msg, MSG_CMSG_CLOEXEC);
            if(var == -1 && errno == EINTR) continue;
            else if(var == -1){
                throw std::system_error(errno, std::generic_category(), "recieve"); 
            }
            else if (var == 0){
                throw std::runtime_error("connection closed");
            }
            else{
                processFDs(msg, fds, fdsOldSize);
                if(msg.msg_flags & MSG_TRUNC){
                    throw std::runtime_error("data was truncated");
                }
                recieved += var;
                iov.iov_base = (void*)(message.data() + recieved);
                iov.iov_len -= var;   
            }
        }
        uint16_t messageLen = (message[6] + (message[7] << 8));
        if(messageLen < 8 || messageLen % 4 != 0){
            throw std::runtime_error("corrupted stream");
        }
        message.resize(messageLen);
        iov.iov_base = message.data() + headerLength;
        iov.iov_len = messageLen - headerLength;
        while(recieved < messageLen){
            msg.msg_controllen = requiredBytes;
            ssize_t var = recvmsg(socket.getFD(), &msg, MSG_CMSG_CLOEXEC);
            if(var == -1 && errno == EINTR) continue;
            else if(var == -1){
                throw std::system_error(errno, std::generic_category(), "recieve"); 
            }
            else if (var == 0){
                throw std::runtime_error("connection closed");
            }
            else {
                processFDs(msg, fds, fdsOldSize);
                if(msg.msg_flags & MSG_TRUNC){
                    throw std::runtime_error("data was truncated");
                }
                recieved += var;
                iov.iov_base = (void*)(message.data() + recieved);
                iov.iov_len -= var;  
            }
        }
        RecievedMessage res;
        res.message = std::move(message);
        res.fds = std::move(fds);
        return res;
    }
};