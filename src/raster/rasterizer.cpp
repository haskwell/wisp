#include "rasterizer.hpp"

#include <algorithm>
#include <cmath>

void Rasterizer::setPixel(Vec2 p, wisp::Color c) {
    uint32_t* pixel = getPixel(p);
    if (pixel) *pixel = c.value;
}

uint32_t* Rasterizer::getPixel(Vec2 p) {
    if (!frameBuffer.pixelData) return nullptr;
    uint32_t pixelsPerRow = frameBuffer.stride / 4;
    int px = static_cast<int>(std::floor(p.getX()));
    int py = static_cast<int>(std::floor(p.getY()));
    if (px < 0 || py < 0 ||
        px >= static_cast<int>(frameBuffer.width) ||
        py >= static_cast<int>(frameBuffer.height)) {
        return nullptr;
    }
    return (frameBuffer.pixelData + px) + (py * pixelsPerRow);
}

void Rasterizer::drawPoint(Vec2 p, wisp::Color c) {
    setPixel(p, c);
}

void Rasterizer::setFramebuffer(FrameBuffer f) {
    frameBuffer = f;
}

void Rasterizer::clearFramebuffer(wisp::Color c) {
    if (!frameBuffer.pixelData) return;
    uint32_t pixelsPerRow = frameBuffer.stride / 4;
    for (uint32_t y = 0; y < frameBuffer.height; y++) {
        uint32_t* row = frameBuffer.pixelData + y * pixelsPerRow;
        std::fill(row, row + frameBuffer.width, c.value);
    }
}
