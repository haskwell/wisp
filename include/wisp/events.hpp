#pragma once

#include <cstdint>
#include <vector>
#include <string>
enum class EventType {
    P_ENTER, P_LEAVE, MOTION, BUTTON, AXIS,
    K_ENTER, K_LEAVE, KEY
};

struct Events {
    EventType type{};
    uint32_t timestamp{};
    uint32_t surfaceID{};
    std::vector<uint32_t> keys;
    std::string utf8;
    union {
        //pointer events
        struct { uint32_t serial; double x, y; } enter;
        struct { uint32_t serial; } leave;
        struct { double x, y; } motion;
        struct { uint32_t serial, button, state; double x, y; } button;
        struct { uint32_t axis; double value; } axis;

        //keyboard events
        struct {uint32_t serial; } k_enter;
        struct {uint32_t serial; } k_leave;
        struct {uint32_t serial; uint32_t keyCode; uint32_t state; uint32_t keysym; } key;
    };
};
