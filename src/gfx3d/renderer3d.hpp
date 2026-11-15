#pragma once
#include <wisp/window.hpp>
#include "../raster/rasterizer.hpp"
class Object3d;
class Vec3;
namespace wisp::gfx3d {
    class Renderer {
        Rasterizer rasterizer;
        Window& window;
        int distanceCamera;
        int FOV;
        int focalLength;
    public:
        Renderer(Window& window, int, int);
        void beginFrame(Color c = Color::Black);
        void endFrame();
        void drawPoint(Vec2 p, Color c = Color::White);
        void drawLine(Vec2 p1, Vec2 p2, Color c = Color::White);
        void project(Vec3, float&, float&);
        void draw(const Object3d&, Color c);
    };
}
