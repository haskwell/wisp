#include "rasterizer.hpp"

#include <algorithm>
#include <cmath>

void Rasterizer::setPixel(Vec2 p, wisp::Color c) {
    uint32_t* pixel = getPixel(p);
    if (pixel) *pixel = c.value;
}

void Rasterizer::setPixel(int x, int y, wisp::Color c) {
    uint32_t* pixel = getPixel(Vec2(x, y));
    if (pixel) *pixel = c.value;
}

uint32_t* Rasterizer::getPixel(Vec2 p) {
    if (!frameBuffer.pixelData) return nullptr;
    uint32_t pixelsPerRow = frameBuffer.stride / 4;
    int px = p.getXi();
    int py = p.getYi();
    if (px < 0 || py < 0 ||
        px >= static_cast<int>(frameBuffer.width) ||
        py >= static_cast<int>(frameBuffer.height)) {
        return nullptr;
    }
    return (frameBuffer.pixelData + px) + (py * pixelsPerRow);
}

void Rasterizer::drawLine(Vec2 p1, Vec2 p2, wisp::Color c) {
    int x1 = p1.getXi();
    int y1 = p1.getYi();
    int x2 = p2.getXi();
    int y2 = p2.getYi();

    bool isSteep = std::abs(y2 - y1) > std::abs(x2 - x1);
    if (isSteep) {
        std::swap(x1, y1);
        std::swap(x2, y2);
    }

    if (x1 > x2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }

    int dx = x2 - x1;
    int dy = std::abs(y2 - y1);

    int d = 2 * dy - dx;

    int y = y1;
    int step = (y2 > y1) ? 1 : -1;

    for (int x = x1; x <= x2; x++) {
        if (isSteep) setPixel(y, x, c);
        else setPixel(x, y, c);
        if (d > 0) {
            y += step;
            d -= 2 * dx;
        }
        d += 2 * dy;
    }
}

void Rasterizer::drawPoint(Vec2 p, wisp::Color c) {
    setPixel(p, c);
}

void Rasterizer::drawTriangle(Vec2 p1, Vec2 p2, Vec2 p3, wisp::Color c) {
    drawLine(p1, p2, c);
    drawLine(p1, p3, c);
    drawLine(p2, p3, c);
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
