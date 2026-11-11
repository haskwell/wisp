#pragma once
#include <sys/socket.h>
#include <cstddef>
#include <span>
#include <cstdint>
#include <vector>
#include <cstddef>
#include <deque>
#include <string>

class Socket{
    int fd = -1;
    std::string path;

    public:
    Socket();

    Socket(int existingFD);

    Socket(const Socket&) = delete;
    Socket& operator=(const Socket&) = delete;

    int getFD() const;

    const std::string& getPath() const;

    ~Socket();
};

class Connection{
    private:
    Socket socket;
    std::deque<int> pendingFds;
    void processFDs(struct msghdr &msg);

    public:
    Connection(int existingFD);

    Connection();
    int takeFd();
    void sendMessage(const std::span<const uint8_t> message, const std::span<const int> fd = {});
    int getFD() const;
    std::vector<uint8_t> receiveMessage();
    ~Connection();
};