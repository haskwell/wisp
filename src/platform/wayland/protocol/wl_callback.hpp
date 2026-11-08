#pragma once
#include "../core/proxy.hpp"

class Callback : public Proxy {
    public:
    Callback(Display& d) : Proxy(d) {}
    bool done = false;
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;
};