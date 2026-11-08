#include "wl_shm_pool.hpp"
#include "wl_buffer.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<Buffer> ShmPool::createBuffer(int32_t width, int32_t height, int32_t stride){
    auto s = std::make_unique<Buffer>(display);
    uint32_t id = display.objectManager.registerProxy(s.get());
    display.sendRequest(objectId, wl_shm_pool::Request::CREATE_BUFFER, id,
        static_cast<int32_t>(0), width, height, stride,
        static_cast<uint32_t>(wl_shm::Format::ARGG8888));
    return s;
}
