#pragma once
#include "platform/wayland/core/proxy.hpp"
#include <utility>
#include <wisp/events.hpp>
class Pointer : public Proxy {
public:
    uint32_t currentSurfaceID{};
    std::pair<double, double> position{};
    std::vector<wisp::Events> pendingFrame;
    Pointer(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override;

    void flushFrame();

    void destroy();
};