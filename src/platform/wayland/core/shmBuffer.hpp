#pragma once

#include <memory>
#include "../protocol/wl_shm_pool.hpp"
#include "../protocol/wl_buffer.hpp"
#include "../protocol/wl_shm.hpp"

class ShmBuffer{
    int fd{-1};
    void* data{nullptr};
    Shm& shm;
    int32_t width;
    int32_t height;
    uint32_t stride;
    uint32_t size;
    const size_t bufferCount{2};
    std::unique_ptr<ShmPool> pool;
    std::vector<std::unique_ptr<Buffer>> buffers;
public:
    ShmBuffer(Shm& shm, uint32_t width, uint32_t height);
    void initialize();
    Buffer& getBuffer(int i) const;
    uint32_t* getPixels(int index) const;
    bool anyBusy();
    int acquireFreeBuffer();
    ~ShmBuffer();
};