#pragma once

#include <unordered_set>

#include "platform/wayland/core/proxy.hpp"
#include "opcodes.hpp"
class Keyboard : public Proxy {
public:
    uint32_t serial;
    uint32_t surfaceId;
    uint32_t key;
    wl_keyboard::KeyState state;
    std::unordered_set<uint32_t> pressedKeys;
    Keyboard(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override;

    void destroy();
};
