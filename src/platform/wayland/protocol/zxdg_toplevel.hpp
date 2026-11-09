#pragma once
#include "../core/proxy.hpp"

class ZxdgTopLevel : public Proxy {
    public:
    ZxdgTopLevel(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;

    void destroy();

    void setModeServer();
};