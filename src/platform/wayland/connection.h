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

std::string get_path() {
    const char* display = std::getenv("WAYLAND_DISPLAY");

    if (!display || display[0] == '\0') display = "wayland-0";
    
    std::string path{};

    if (display[0] == '/'){
        path = display;
    }
    else{
        const char* runtime = std::getenv("XDG_RUNTIME_DIR");
        if(!runtime){
            throw std::runtime_error("'XDG_RUNTIME_DIR' not set");
        }
        path = runtime;
        path += '/';
        path += display;
    }
    return path;
}

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

    void sendMessage(const std::span<const uint8_t> message){
        size_t length = message.size();
        size_t sent = 0;
        const uint8_t* ptr = message.data();
        while(sent < length){
            ssize_t var = send(socket.getFD(), ptr + sent, length - sent, MSG_NOSIGNAL);
            if(var == -1 && errno == EINTR) continue;
            else if (var == -1){
                throw std::system_error(errno, std::generic_category(), "send");
            }
            else if(var == 0){
                throw std::runtime_error("connection unexpectedly closed");
            }
            else sent += var;
        }
    }

    std::vector<uint8_t> receiveMessage(){
        std::vector<uint8_t> message(8);
        size_t headerLength = 8;
        size_t recieved = 0;
        while(recieved < headerLength){
            ssize_t var = recv(socket.getFD(), message.data() + recieved, headerLength - recieved, 0);
            if(var == -1 && errno == EINTR) continue;
            else if(var == -1){
                throw std::system_error(errno, std::generic_category(), "recieve"); 
            }
            else if (var == 0){
                throw std::runtime_error("connection closed");
            }
            else recieved += var;
        }
        uint16_t messageLen = (message[6] + (message[7] << 8));
        if(messageLen < 8 || messageLen % 4 != 0){
            throw std::runtime_error("corrupted stream");
        }
        message.resize(messageLen);
        while(recieved < messageLen){
            ssize_t var = recv(socket.getFD(), message.data() + recieved, messageLen - recieved, 0);
            if(var == -1 && errno == EINTR) continue;
            else if(var == -1){
                throw std::system_error(errno, std::generic_category(), "recieve"); 
            }
            else if (var == 0){
                throw std::runtime_error("connection closed");
            }
            else recieved += var;
        }
        return message;
    }
};