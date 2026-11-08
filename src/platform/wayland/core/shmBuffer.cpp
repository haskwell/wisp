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

    int stride = width * 4;
    uint32_t size = width * height * 4;
    ftruncate(fd, size);

    void* shm_data = mmap(nullptr, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

    uint32_t* pixels = static_cast<uint32_t*>(shm_data);
    for(size_t i = 0; i < static_cast<size_t>(width) * height; i++){
        pixels[i] = 0xFF3366CC;
    }

    pool = shm.createShmPool(size, fd);
    buffer = pool->createBuffer(width, height, stride);
}

Buffer& ShmBuffer::getBuffer(){
    return *buffer;
}

uint32_t* getPixels();

ShmBuffer::~ShmBuffer(){
    munmap(data, size);
    close(fd);
}