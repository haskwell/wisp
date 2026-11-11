#include <cstring>
#include <unistd.h>
#include <cerrno>
#include <sys/uio.h>
#include <sys/un.h>
#include <utility>
#include <stdexcept>
#include <cstdlib>
#include <system_error>

#include "wayland_utils.hpp"
#include "connection.hpp"
#include <deque>
Socket::Socket(){
    
    path = get_path();
    
    if(path.length() + 1 > sizeof(sockaddr_un::sun_path)){
        throw std::runtime_error("Path length too long");
    }

    fd = socket(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0);
    if(fd < 0){
        throw std::system_error(errno, std::generic_category(),"Socket creation failed");
    }
}

Socket::Socket(int existingFD) : fd(existingFD) {}

int Socket::getFD() const{
    return fd;
}

const std::string& Socket::getPath() const{
    return path;
}

Socket::~Socket(){
    close(fd);
}


Connection::Connection(int existingFD) : socket(existingFD) {}
Connection::Connection(){
    sockaddr_un addr{};
    addr.sun_family = AF_UNIX;

    strcpy(addr.sun_path, socket.getPath().c_str());
    
    socklen_t socklen = offsetof(struct sockaddr_un, sun_path) + socket.getPath().length() + 1;
    sockaddr* saddr = reinterpret_cast<sockaddr*>(&addr);
    
    if(connect(socket.getFD(), saddr, socklen) < 0){
        throw std::system_error(errno, std::generic_category(), "Connection failed");
    }
}

int Connection::takeFd() {
    if (pendingFds.empty()) {
        throw std::runtime_error("expected an fd but none was received");
    }
    int fd = pendingFds.front();
    pendingFds.pop_front();
    return fd;
}

void Connection::sendMessage(const std::span<const uint8_t> message, const std::span<const int> fd){

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

int Connection::getFD() const {
    return socket.getFD();
}

void Connection::processFDs(struct msghdr &msg){
    if(msg.msg_flags & MSG_CTRUNC){
        throw std::runtime_error("not enough space for all FDs");
    }
    for (cmsghdr* c = CMSG_FIRSTHDR(&msg); c != nullptr; c = CMSG_NXTHDR(&msg, c)) {
        if (c->cmsg_level != SOL_SOCKET || c->cmsg_type != SCM_RIGHTS) continue;
        if (c->cmsg_len < CMSG_LEN(0)) continue;
        size_t dataLen = c->cmsg_len - CMSG_LEN(0);
        if (dataLen % sizeof(int) != 0) {
            throw std::runtime_error("corrupted stream");
        }
        size_t n = dataLen / sizeof(int);
        const unsigned char* data = CMSG_DATA(c);
        for (size_t i = 0; i < n; ++i) {
            int fd;
            memcpy(&fd, data + i * sizeof(int), sizeof(int));  // avoids alignment issues
            pendingFds.push_back(fd);
        }
    }
}

std::vector<uint8_t> Connection::receiveMessage(){
    const size_t MAX_FDS = 16;
    std::vector<uint8_t> message(8);

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
            processFDs(msg);
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
            processFDs(msg);
            if(msg.msg_flags & MSG_TRUNC){
                throw std::runtime_error("data was truncated");
            }
            recieved += var;
            iov.iov_base = (void*)(message.data() + recieved);
            iov.iov_len -= var;  
        }
    }
    return message;
}

Connection::~Connection() {
    for (int fd : pendingFds) close(fd);
}
