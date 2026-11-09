#pragma once
#include <functional>
#include "../core/proxy.hpp"
#include <memory>

class XdgTopLevel;
class XdgSurface : public Proxy {
    public:
    bool configured = false;
    std::function<void()> onConfigure;
    XdgSurface(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;

    void destroy();

    std::unique_ptr<XdgTopLevel> getToplevel();
};