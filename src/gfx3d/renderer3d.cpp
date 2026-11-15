#include "renderer3d.hpp"
#include <cmath>

#include "object3d.hpp"
#include "../math/vec3.hpp"
using namespace wisp::gfx3d;
Renderer::Renderer(Window &w, int fov, int d) : window(w) {
    FOV = fov;
    distanceCamera = d;
    uint32_t height = window.getHeight();
    focalLength = (height / 2) / tan((FOV / 2));
}

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

void Renderer::project(Vec3 p, float& px, float &py) {
    float z = p.getZ() + distanceCamera;
    uint32_t width = window.getWidth();
    uint32_t height = window.getHeight();
    float cx = width / 2.f;
    float cy = height / 2.f;
    px = cx + (focalLength * (p.getX() / z));
    py = cy - (focalLength * (p.getY() / z));
}

void Renderer::draw(const Object3d& o, Color c) {
    int vertices = o.points.size();
    std::vector<Vec2> projectedPoints;
    projectedPoints.reserve(vertices);
    for (int i = 0; i < vertices; i++) {
        float px, py;
        project(o.points[i], px, py);
        projectedPoints.push_back(Vec2(px, py));
    }
    int edges = o.edges.size();
    for (int i = 0; i < edges; i++) {
        int i1 = o.edges[i].first;
        int i2 = o.edges[i].second;
        Vec2 p1 = projectedPoints[i1];
        Vec2 p2 = projectedPoints[i2];
        drawLine(p1, p2, c);
    }
}
