#include "wl_keyboard.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/mman.h>

#include "opcodes.hpp"
#include "platform/wayland/core/display.hpp"

void Keyboard::destroy() {
    display.sendRequest(objectId, wl_keyboard::Request::RELEASE);
}

void Keyboard::handleEvent(uint16_t opcode, MessageDeserializer& d) {
    if (opcode == static_cast<uint16_t>(wl_keyboard::Event::ENTER)) {
        Events event{};
        event.type = EventType::K_ENTER;
        pressedKeys.clear();
        event.k_enter.serial = d.readUnsignedInteger<uint32_t>();
        event.surfaceID = d.readUnsignedInteger<uint32_t>();
        surfaceId = event.surfaceID;
        std::vector<uint32_t> pressedKeysVector = d.readArray32bit();
        pressedKeys.assign(pressedKeysVector.begin(), pressedKeysVector.end());
        event.keys.assign(pressedKeys.begin(), pressedKeys.end());
        sendEvent(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_keyboard::Event::LEAVE)) {
        Events event{};
        event.type = EventType::K_LEAVE;
        event.k_leave.serial = d.readUnsignedInteger<uint32_t>();
        event.surfaceID = d.readUnsignedInteger<uint32_t>();
        surfaceId = 0;
        pressedKeys.clear();
        event.keys.clear();
        sendEvent(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_keyboard::Event::KEY)) {
        Events event{};
        event.type = EventType::KEY;
        event.key.serial = d.readUnsignedInteger<uint32_t>();
        event.timestamp = d.readUnsignedInteger<uint32_t>();
        uint32_t key = d.readUnsignedInteger<uint32_t>();
        event.key.keyCode = key;
        state = static_cast<wl_keyboard::KeyState>(
            d.readUnsignedInteger<uint32_t>()
        );
        if (state == wl_keyboard::KeyState::PRESSED) {
            pressedKeys.push_back(key);
            event.key.state = 1;
            uint32_t keySym = xkb.getKeySym(key);
            event.utf8 = xkb.getUTF8(key);
            event.key.keysym = keySym;
        }
        if (state == wl_keyboard::KeyState::RELEASED) {
            std::erase(pressedKeys, key);
            event.key.state = 0;
            uint32_t keySym = xkb.getKeySym(key);
            event.utf8 = xkb.getUTF8(key);
            event.key.keysym = keySym;
        }
        event.keys.assign(pressedKeys.begin(), pressedKeys.end());
        event.surfaceID = surfaceId;
        sendEvent(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_keyboard::Event::KEYMAP)) {
        uint32_t format = d.readUnsignedInteger<uint32_t>();
        uint32_t size = d.readUnsignedInteger<uint32_t>();
        int fd = display.takeFd();
        keymapFormat = format;
        keymapData.clear();
        if (size > 0) {
            void* map = mmap(nullptr, size, PROT_READ, MAP_PRIVATE, fd, 0);
            if (map != MAP_FAILED) {
                keymapData.assign(static_cast<char*>(map), static_cast<char*>(map) + size);
                munmap(map, size);
            }
        }

        xkb.initialize(keymapData.data());
        close(fd);
    }
    else if (opcode == static_cast<uint16_t>(wl_keyboard::Event::MODIFIERS)) {
        d.readUnsignedInteger<uint32_t>();
        uint32_t depressed = d.readUnsignedInteger<uint32_t>();
        uint32_t latched = d.readUnsignedInteger<uint32_t>();
        uint32_t locked = d.readUnsignedInteger<uint32_t>();
        uint32_t group = d.readUnsignedInteger<uint32_t>();
        xkb.updateModifierState(depressed, latched, locked, group);
    }
}

void Keyboard::sendEvent(Events e) {
    display.addEvent(e);
}