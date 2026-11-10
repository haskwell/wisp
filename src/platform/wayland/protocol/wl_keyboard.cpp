#include "wl_keyboard.hpp"

#include <iostream>

#include "opcodes.hpp"
#include "platform/wayland/core/display.hpp"

void Keyboard::destroy() {
    display.sendRequest(objectId, wl_keyboard::Request::RELEASE);
}

void Keyboard::handleEvent(uint16_t opcode, MessageDeserializer& d) {
    if (opcode == static_cast<uint16_t>(wl_keyboard::Event::ENTER)) {
        pressedKeys.clear();
        serial = d.readUnsignedInteger<uint32_t>();
        surfaceId = d.readUnsignedInteger<uint32_t>();
        std::vector<uint32_t> pressedKeysVector = d.readArray32bit();
        pressedKeys.insert(pressedKeysVector.begin(), pressedKeysVector.end());
    }
    else if (opcode == static_cast<uint16_t>(wl_keyboard::Event::LEAVE)) {
        surfaceId = 0;
        pressedKeys.clear();
    }
    else if (opcode == static_cast<uint16_t>(wl_keyboard::Event::KEY)) {
        serial = d.readUnsignedInteger<uint32_t>();
        d.readUnsignedInteger<uint32_t>();
        key = d.readUnsignedInteger<uint32_t>();
        state = static_cast<wl_keyboard::KeyState>(
            d.readUnsignedInteger<uint32_t>()
        );
        if (state == wl_keyboard::KeyState::PRESSED) {
            pressedKeys.insert(key);
        }
        if (state == wl_keyboard::KeyState::RELEASED) {
            pressedKeys.erase(key);
        }
    }
}
