#pragma once
#include "../core/proxy.hpp"

class Buffer : public Proxy {
    public:
    Buffer(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
};