#pragma once
#include <cmath>

class Vec2 {
    float x{};
    float y{};
    public:
    Vec2() = default;
    Vec2(float x_, float y_) : x(x_), y(y_) {}
    void set(float x, float y) {
        this->x = x;
        this->y = y;
    }
    float getX() {return x;}
    float getY() {return y;}
    int getXi() const { return static_cast<int>(std::floor(x)); }
    int getYi() const { return static_cast<int>(std::floor(y)); }
    void setX(float x_) {x = x_;}
    void setY(float y_) {y=y_;}
};
