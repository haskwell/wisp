#pragma once

#include <cstdint>
#include <vector>
#include <string>
namespace wisp {

    enum class Key : uint16_t {
        Unknown = 0,
        Left, Right, Up, Down,
        Space, Enter, Escape, Tab, Backspace,
        A, B, C, D, E, F, G, H, I, J, K, L, M,
        N, O, P, Q, R, S, T, U, V, W, X, Y, Z,
        Num0, Num1, Num2, Num3, Num4, Num5, Num6, Num7, Num8, Num9,
        LShift, RShift, LCtrl, RCtrl, LAlt, RAlt,
        F1, F2, F3, F4, F5, F6, F7, F8, F9, F10, F11, F12,
    };

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
            struct {uint32_t serial; uint32_t keyCode; uint32_t state; uint32_t keysym; Key key;     } key;
        };
    };
}