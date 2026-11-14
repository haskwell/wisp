#include "renderer2d.hpp"

using namespace wisp::gfx2d;
Renderer::Renderer(Window &w) : window(w) {}

void Renderer::beginFrame(Color c) {
    rasterizer.setFramebuffer(window.getFrameBuffer());
    rasterizer.clearFramebuffer(c);
}

void Renderer::endFrame() {
    window.update();
}

void Renderer::drawPoint(Vec2 p, Color c) {
    rasterizer.drawPoint(p, c);
}

void Renderer::drawLine(Vec2 p1, Vec2 p2, Color c) {
    rasterizer.drawLine(p1, p2, c);
}