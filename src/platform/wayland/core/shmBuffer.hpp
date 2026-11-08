#include <memory>
#include "../protocol/wl_shm_pool.hpp"
#include "../protocol/wl_buffer.hpp"
#include "../protocol/wl_shm.hpp"

class ShmBuffer{
    int fd{-1};
    void* data;
    Shm& shm;
    int32_t width;
    int32_t height;
    uint32_t stride;
    uint32_t size;

    std::unique_ptr<ShmPool> pool;
    std::unique_ptr<Buffer> buffer;
public:
    ShmBuffer(Shm& shm, uint32_t width, uint32_t height);
    void initialize();
    Buffer& getBuffer();
    uint32_t* getPixels();
    ~ShmBuffer();
};