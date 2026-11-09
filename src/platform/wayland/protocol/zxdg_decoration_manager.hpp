#pragma once
#include "../core/proxy.hpp"
#include <memory>

class ZxdgTopLevel;

class ZxdgDecorManager : public Proxy {
    public:
    ZxdgDecorManager(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override{}
    std::unique_ptr<ZxdgTopLevel> getZxdgToplevel(uint32_t xdgTopLevelId);
};