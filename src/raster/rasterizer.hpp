#pragma once
#include <wisp/frameBuffer.hpp>
#include "../math/vec2.hpp"
#include "../math/color.hpp"
class Rasterizer {
    FrameBuffer frameBuffer{};
    void setPixel(Vec2 p, wisp::Color c);
    uint32_t* getPixel(Vec2 p);
public:
    void drawLine(Vec2, Vec2, wisp::Color);
    void drawLineAA(Vec2, Vec2, wisp::Color);
    void drawPoint(Vec2, wisp::Color);
    void drawTriangle(Vec2, Vec2, Vec2, wisp::Color);
    void drawTriangleFill(Vec2, Vec2, Vec2, wisp::Color, wisp::Color);
    void setFramebuffer(FrameBuffer);
    void clearFramebuffer(wisp::Color c);
};