#pragma once
#include <sys/socket.h>
#include <cstddef>
#include <span>
#include <cstdint>
#include <vector>
#include <cstddef>

struct RecievedMessage{
    std::vector<uint8_t> message;
    std::vector<int> fds;
};

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

    void processFDs(struct msghdr &msg, std::vector<int>& fds, size_t& fdsOldSize);

    public:
    Connection(int existingFD);

    Connection();

    void sendMessage(const std::span<const uint8_t> message, const std::span<const int> fd);

    RecievedMessage receiveMessage();
};