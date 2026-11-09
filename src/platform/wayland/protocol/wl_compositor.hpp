#pragma once
#include "../core/proxy.hpp"
#include <memory>

class Surface;

class Compositor : public Proxy {
    public:
    Compositor(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override{}
    std::unique_ptr<Surface> createSurface();
};