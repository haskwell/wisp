#pragma once
#include "../core/proxy.hpp"
#include <memory>

class Registry;

class Callback;

class WlDisplay : public Proxy {
    public:
    WlDisplay(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;

    std::unique_ptr<Registry> getRegistry();
    std::unique_ptr<Callback> sync();
};