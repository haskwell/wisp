#pragma once

#include "platform/wayland/core/proxy.hpp"
#include "opcodes.hpp"
#include "platform/wayland/core/xkb.hpp"
#include <wisp/events.hpp>
class Keyboard : public Proxy {
public:
    uint32_t serial;
    uint32_t surfaceId;
    uint32_t key;
    wl_keyboard::KeyState state;
    std::vector<uint32_t> pressedKeys;
    uint32_t keymapFormat = 0;
    std::vector<char> keymapData;
    XKB xkb;
    Keyboard(Display& d) : Proxy(d) {}
    void handleEvent(
        uint16_t opcode,
        MessageDeserializer& d
    ) override;

    void sendEvent(wisp::Events e);

    void destroy();
};
