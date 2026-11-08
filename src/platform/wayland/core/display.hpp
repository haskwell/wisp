#pragma once
#include "../wire/connection.hpp"
#include "../wire/serializer.hpp"
#include "../wire/deserializer.hpp"
#include "object_manager.hpp"
#include "../protocol/wl_display.hpp"

class Display{
        public:
    
    Connection connection;
    MessageDeserializer deserializer;
    MessageSerializer serializer;
    ObjectManager objectManager;

    WlDisplay wldisplay{*this};
    Display();
    template <typename Opcode, typename... Args>
    void sendRequest(uint32_t objectId, Opcode opcode, Args&... args){
        serializer.beginMessage(objectId, opcode);
        (appendArg(args),...);
        serializer.finishMessage();
        send();
    }

    void send();
    void dispatch();
    void roundTrip();

    std::unique_ptr<Registry> getRegistry();

    void appendArg(uint32_t a);
    void appendArg(int32_t a);
    void appendArg(std::string a);
};