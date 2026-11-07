
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <system_error>
#include <cerrno>
#include <cstring>
#include "platform/wayland/connection.hpp"

int main() {
    int sockets[2];

    if (socketpair(AF_UNIX, SOCK_STREAM | SOCK_CLOEXEC, 0, sockets) == -1)
        throw std::system_error(errno, std::generic_category(), "socketpair");

    Connection sender(sockets[0]);
    Connection receiver(sockets[1]);

    int pipefd[2];

    if (pipe2(pipefd, O_CLOEXEC) == -1)
        throw std::system_error(errno, std::generic_category(), "pipe2");

    // Send a valid 8-byte Wayland-style message and one FD.
    std::vector<uint8_t> message{
        1, 0, 0, 0,  // object ID = 1
        0, 0,        // opcode = 0
        8, 0         // message length = 8
    };

    std::span<const int> fds(&pipefd[1], 1);

    sender.sendMessage(message, fds);

    auto received = receiver.receiveMessage();

    std::cout << "Message size: " << received.message.size() << '\n';
    std::cout << "Received FDs: " << received.fds.size() << '\n';

    if (received.message != message || received.fds.size() != 1)
        throw std::runtime_error("Transport test failed");

    // Write through the received FD.
    const char testByte = 'Z';

    if (write(received.fds[0], &testByte, 1) != 1)
        throw std::system_error(errno, std::generic_category(), "write");

    // Read through the original pipe read end.
    char result{};

    if (read(pipefd[0], &result, 1) != 1)
        throw std::system_error(errno, std::generic_category(), "read");

    std::cout << "Pipe received: " << result << '\n';

    if (result != 'Z')
        throw std::runtime_error("FD transfer failed");

    std::cout << "FD TRANSFER SUCCESS!\n";

    // Close received and original descriptors.
    for (int fd : received.fds)
        close(fd);

    close(pipefd[0]);
    close(pipefd[1]);

    return 0;
}
