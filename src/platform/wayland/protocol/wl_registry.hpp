#pragma once
#include "../core/proxy.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"
#include <vector>
#include <memory>
#include <string>
struct Global{
    uint32_t name;
    std::string interface;
    uint32_t version;
};

class Registry : public Proxy {
    std::vector<Global> globals;
    public:
    Registry(Display& d) : Proxy(d) {}
    const Global* findGlobal(std::string interface);
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;
    std::vector<Global> getGlobals();

    template <typename T>
    std::unique_ptr<T> bind(std::string interface){
        const Global* g = findGlobal(interface);
        auto proxy = std::make_unique<T>(display);
        uint32_t id = display.objectManager.registerProxy(proxy.get());
        display.sendRequest(objectId, wl_registry::Request::BIND, g->name, g->interface, g->version, id);
        return proxy;
    }
};