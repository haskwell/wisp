#pragma once
#include "../core/proxy.hpp"
#include <memory>
class ShmPool;
class Shm : public Proxy {
    public:
    Shm(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override{}
    std::unique_ptr<ShmPool> createShmPool(uint32_t size, int fd);
};