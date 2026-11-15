#pragma once
#include <cmath>

class Vec3 {
    float x{};
    float y{};
    float z{};
public:
    Vec3() = default;
    Vec3(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    void set(float x, float y, float z) {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    float getX() { return x; }
    float getY() { return y; }
    float getZ() { return z; }

    int getXi() const { return static_cast<int>(std::floor(x)); }
    int getYi() const { return static_cast<int>(std::floor(y)); }
    int getZi() const { return static_cast<int>(std::floor(z)); }

    void setX(float x_) { x = x_; }
    void setY(float y_) { y = y_; }
    void setZ(float z_) { z = z_; }
};