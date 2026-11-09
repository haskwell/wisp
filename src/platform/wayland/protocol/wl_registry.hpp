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
    const std::vector<Global>& getGlobals() const;

    template <typename T>
    std::unique_ptr<T> bind(std::string interface, uint32_t maxVersion, bool required = true){
        const Global* g = findGlobal(interface);
        if (!g) {
            if (required) {
                throw std::runtime_error("compositor does not support global" + interface);
            }
            return nullptr;
        }
        uint32_t version = std::min(g->version, maxVersion);
        auto proxy = display.registerProxy<T>();
        display.sendRequest(objectId, wl_registry::Request::BIND, g->name, g->interface, version, proxy->objectId);
        return proxy;
    }
};