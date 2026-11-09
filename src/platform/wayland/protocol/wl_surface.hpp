#pragma once
#include "../core/proxy.hpp"
#include "wl_callback.hpp"
#include <memory>
class Callback;
class Surface : public Proxy {
    public:
    Surface(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override{}
    void commit();
    std::unique_ptr<Callback> frame();
    void sendAttachRequest(uint32_t bufferId, int32_t x, int32_t y);
    void sendDamageBufferRequest(int32_t x, int32_t y, int32_t width, int32_t height);

    void destroy();
};