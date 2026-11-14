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
