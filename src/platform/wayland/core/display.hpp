#pragma once
#include "../wire/connection.hpp"
#include "../wire/serializer.hpp"
#include "../wire/deserializer.hpp"
#include "object_manager.hpp"
#include "../protocol/wl_display.hpp"
#include <memory>

class Display{
    Connection connection;
    MessageDeserializer deserializer;
    MessageSerializer serializer;
    ObjectManager objectManager;
public:
    bool running = true;
    WlDisplay wldisplay{*this};
    std::unique_ptr<Registry> registry;
    Display();
    template <typename Opcode, typename... Args>
    void sendRequest(uint32_t objectId, Opcode opcode, const Args&... args){
        sendRequestWithFds(objectId, opcode, std::span<const int>{}, args...);
    }

    template <typename Opcode, typename... Args>
    void sendRequestWithFds(uint32_t objectId, Opcode opcode, std::span<const int> fds, const Args&... args){
        serializer.beginMessage(objectId, opcode);
        (appendArg(args),...);
        serializer.finishMessage();
        send(fds);
    }

    template <typename T>
    std::unique_ptr<T> registerProxy(){
        auto p = std::make_unique<T>(*this);
        objectManager.registerProxy(p.get());   // sets p->objectId
        return p;
    }

    void send(std::span<const int> fds = {});
    void dispatch();
    void roundTrip();

    Registry& getRegistry();

    void appendArg(uint32_t a);
    void appendArg(int32_t a);
    void appendArg(std::string a);

    void unregisterProxy(uint32_t id);

    void deleteProxy(uint32_t id);

    void run();
    void stop();

    ~Display();
};