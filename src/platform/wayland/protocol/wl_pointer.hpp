#pragma once

#include "platform/wayland/core/proxy.hpp"

class Pointer : public Proxy {
public:
    uint32_t serial;
    uint32_t surfaceId;
    bool insideSurface;
    double x;
    double y;
    double scrollX;
    double scrollY;
    uint32_t lastButton;
    uint32_t lastButtonState;
    Pointer(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override;

    void destroy();
};