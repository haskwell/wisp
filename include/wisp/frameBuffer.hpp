#pragma once
#include <cstdint>

struct FrameBuffer {
    uint32_t* pixelData;
    uint32_t width;
    uint32_t height;
    uint32_t stride;
};
