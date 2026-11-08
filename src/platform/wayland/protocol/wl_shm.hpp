#pragma once
#include "../core/proxy.hpp"

class SHM : public Proxy {
    public:
    SHM(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
};