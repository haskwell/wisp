#pragma once
#include "../core/proxy.hpp"
#include <memory>

class XdgTopLevel;
class XdgSurface : public Proxy {
    public:
    bool configured = false;
    XdgSurface(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;
    std::unique_ptr<XdgTopLevel> getToplevel();
};