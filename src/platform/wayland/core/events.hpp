#pragma once

#include <cstdint>

enum class EventType {
    ENTER, LEAVE, MOTION, BUTTON, AXIS
};

struct Events {
    EventType type{};
    uint32_t timestamp{};
    uint32_t surfaceID{};
    union {
        struct { uint32_t serial; double x, y; } enter;
        struct { uint32_t serial; } leave;
        struct { double x, y; } motion;
        struct { uint32_t serial, button, state; double x, y; } button;
        struct { uint32_t axis; double value; } axis;
    };
};
