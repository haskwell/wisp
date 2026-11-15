#pragma once
#include <vector>
#include "../math/vec3.hpp"
class Object3d {
public:
    std::vector<Vec3> points;
    std::vector<std::pair<int, int>> edges;
    virtual ~Object3d() = default;
};
