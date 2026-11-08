#include "display.hpp"
#include "../protocol/wl_callback.hpp"
#include "../protocol/opcodes.hpp"
#include "../protocol/wl_registry.hpp"

Display::Display(){
    objectManager.registerAt(1, &wldisplay);
}

void Display::send(){
    connection.sendMessage(serializer.getMessage());
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

std::unique_ptr<Registry> Display::getRegistry() {
    return wldisplay.getRegistry();
}
