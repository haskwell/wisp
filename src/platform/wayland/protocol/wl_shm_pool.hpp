#pragma once
#include "../core/proxy.hpp"
#include <memory>
class Buffer;

class ShmPool : public Proxy {
    public:
    ShmPool(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override{}
    std::unique_ptr<Buffer> createBuffer(int32_t width, int32_t height, int32_t stride, int32_t offset);

    void destroy();
};