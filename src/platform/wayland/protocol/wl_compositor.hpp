#pragma once
#include "../core/proxy.hpp"

class Compositor : public Proxy {
    public:
    Compositor(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
};