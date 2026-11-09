#include "display.hpp"
#include "../protocol/wl_callback.hpp"
#include "../protocol/opcodes.hpp"
#include "../protocol/wl_registry.hpp"
#include <unistd.h>
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

void Display::unregisterProxy(uint32_t id){ objectManager.unregisterProxy(id); }
void Display::deleteProxy(uint32_t id){ objectManager.deleteProxy(id); }

Registry& Display::getRegistry() {
    return *registry;
}

void Display::run(){
    dispatch();
}

void Display::stop() {
    running = false;
}

Display::~Display() = default;