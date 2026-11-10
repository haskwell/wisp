#include "display.hpp"
#include "../protocol/wl_callback.hpp"
#include "../protocol/opcodes.hpp"
#include "../protocol/wl_registry.hpp"
#include <unistd.h>
#include <poll.h>
#include <cerrno>
#include <stdexcept>
#include <system_error>

Display::Display(){
    objectManager.registerAt(1, &wldisplay);
    registry = wldisplay.getRegistry();
    //roundTrip();
}

void Display::send(std::span<const int> fds){
    connection.sendMessage(serializer.getMessage(), fds);
}

void Display::dispatch(){
    RecievedMessage res = connection.receiveMessage();
    deserializer.startDeserializer(res.message);
    uint32_t objectID = deserializer.readObjectID();
    uint16_t opcode = deserializer.readOpcode();
    deserializer.readMessageLength();
    Proxy* p = objectManager.lookupProxy(objectID);
    if(p){
        p->handleEvent(opcode, deserializer);
    }
    for (int fd : res.fds) close(fd);
}

void Display::roundTrip(){
    auto callback = wldisplay.sync();
    while(!callback->done){
        dispatch();
    }
}

void Display::appendArg(uint32_t a){
    serializer.append_uint(a);
}

void Display::appendArg(int32_t a){
    serializer.append_int(a);
}

void Display::appendArg(std::string a){
    serializer.appendString(a);
}

void Display::unregisterProxy(uint32_t id, Proxy* p){ objectManager.unregisterProxy(id, p); }
void Display::deleteProxy(uint32_t id){ objectManager.deleteProxy(id); }

Registry& Display::getRegistry() {
    return *registry;
}

void Display::run(){
    pollfd pfd{};
    pfd.fd = connection.getFD();
    pfd.events = POLLIN;
    int timeout = -1;
    while (true) {
        int result = poll(&pfd, 1, timeout);
        if (result < 0) {
            if (errno == EINTR) {
                continue;
            }
            throw std::system_error(errno, std::generic_category(), "poll");
        }
        if (result == 0) break;
        if (pfd.revents & (POLLERR | POLLNVAL)) {
            throw std::runtime_error("wayland socket error");
        }
        if (!(pfd.revents & POLLIN)) {
            throw std::runtime_error("wayland connection closed");
        }
        dispatch();
        timeout = 0;
    }
}

void Display::stop() {
    running = false;
}

Display::~Display() = default;