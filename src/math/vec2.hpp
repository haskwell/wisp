#pragma once

class Vec2 {
    float x{};
    float y{};
    public:
    Vec2(float x_, float y_) : x(x_), y(y_) {}
    void set(float x, float y) {
        this->x = x;
        this->y = y;
    }
    float getX() {return x;}
    float getY() {return y;}
};