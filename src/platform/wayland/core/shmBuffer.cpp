#include "shmBuffer.hpp"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <cstdint>
#include <cstddef>
#include <stdexcept>

ShmBuffer::ShmBuffer(Shm& s, uint32_t w, uint32_t h)
    : shm(s), width(w), height(h) {
    stride = width * 4;
    size = stride * height;
}

void ShmBuffer::initialize(){
    fd = memfd_create("wayland-buffer", MFD_CLOEXEC);
    if(fd < 0){
        throw std::runtime_error("shm creation failed");
    }
    buffers.reserve(bufferCount);
    stride = width * 4;
    size = width * height * 4 * bufferCount;
    if (ftruncate(fd, size) == -1) {
        throw std::runtime_error("Failed to resize file");
    }

    data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (data == MAP_FAILED) {
        throw std::runtime_error("Failed to map memory");
    }

    pool = shm.createShmPool(size, fd);
    int32_t offset = 0;
    for (size_t i = 0; i < bufferCount; i++) {
        offset = i * stride * height;
        buffers.push_back(pool->createBuffer(width, height, stride, offset));
    }
    pool->destroy();
    pool.reset();
}

Buffer& ShmBuffer::getBuffer(int i) const{
    return *(buffers[i]);
}

uint32_t* ShmBuffer::getPixels(int index) const {
    auto* base = static_cast<uint8_t*>(data);
    size_t offset = static_cast<size_t>(index) * stride * static_cast<size_t>(height);
    return reinterpret_cast<uint32_t*>(base + offset);
}

bool ShmBuffer::anyBusy() {
    for (size_t i = 0; i < buffers.size(); i++) {
        if (buffers[i]->busy) {
            return true;
        }
    }
    return false;
}

int ShmBuffer::acquireFreeBuffer() {
    for (size_t i = 0; i < bufferCount; i++) {
        if (!buffers[i]->busy) {
            return i;
        }
    }
    return -1;
}

uint32_t ShmBuffer::getStride() const {
    return stride;
}

ShmBuffer::~ShmBuffer(){
    for (auto& b : buffers) b->destroy();
    if (data) munmap(data, size);
    if (fd >= 0) close(fd);
}
