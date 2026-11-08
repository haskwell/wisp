#pragma once
#include "../core/proxy.hpp"

class XdgBase : public Proxy {
    public:
    XdgBase(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
};