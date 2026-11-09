#include "wl_shm_pool.hpp"
#include "wl_buffer.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<Buffer> ShmPool::createBuffer(int32_t width, int32_t height, int32_t stride, int32_t offset){
    auto s = display.registerProxy<Buffer>();
    display.sendRequest(objectId, wl_shm_pool::Request::CREATE_BUFFER, s->objectId,
        offset, width, height, stride,
        static_cast<uint32_t>(wl_shm::Format::ARGB8888));
    return s;
}

void ShmPool::destroy(){
    display.sendRequest(objectId, wl_shm_pool::Request::DESTROY);
}