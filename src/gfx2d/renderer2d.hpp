#pragma once
#include <wisp/window.hpp>
#include "../raster/rasterizer.hpp"
namespace wisp::gfx2d {
    class Renderer {
        Rasterizer rasterizer;
        Window& window;
        public:
        Renderer(Window& window);
        void beginFrame(Color c = Color::Black);
        void endFrame();
        void drawPoint(Vec2 p, Color c = Color::White);
        void drawLine(Vec2 p1, Vec2 p2, Color c = Color::White);
    };
}
