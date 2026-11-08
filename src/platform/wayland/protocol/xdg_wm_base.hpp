#pragma once
#include "../core/proxy.hpp"
#include <memory>
class XdgSurface;

class XdgBase : public Proxy {
    public:
    XdgBase(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;
    std::unique_ptr<XdgSurface> getXdgSurface(uint32_t surfaceId);
};