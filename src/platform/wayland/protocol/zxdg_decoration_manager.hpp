#pragma once
#include "../core/proxy.hpp"

class ZxdgDecorManager : public Proxy {
    public:
    ZxdgDecorManager(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
};