#pragma once
#include "../core/proxy.hpp"

class Surface : public Proxy {
    public:
    Surface(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
    void commit();
    void sendAttachRequest(uint32_t bufferId, int32_t x, int32_t y);
    void sendDamageBufferRequest(int32_t x, int32_t y, int32_t width, int32_t height);
};