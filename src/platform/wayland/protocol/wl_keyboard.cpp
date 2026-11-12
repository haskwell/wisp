#include "wl_keyboard.hpp"

#include <iostream>
#include <unistd.h>
#include <sys/mman.h>

#include "opcodes.hpp"
#include "platform/wayland/core/display.hpp"
using namespace wisp;
static Key translateKeysym(uint32_t sym) {
    switch (sym) {
        case XKB_KEY_Left:      return Key::Left;
        case XKB_KEY_Right:     return Key::Right;
        case XKB_KEY_Up:        return Key::Up;
        case XKB_KEY_Down:      return Key::Down;
        case XKB_KEY_space:     return Key::Space;
        case XKB_KEY_Return:    return Key::Enter;
        case XKB_KEY_Escape:    return Key::Escape;
        case XKB_KEY_Tab:       return Key::Tab;
        case XKB_KEY_BackSpace: return Key::Backspace;
        case XKB_KEY_Shift_L:   return Key::LShift;
        case XKB_KEY_Shift_R:   return Key::RShift;
        case XKB_KEY_Control_L: return Key::LCtrl;
        case XKB_KEY_Control_R: return Key::RCtrl;
        case XKB_KEY_Alt_L:     return Key::LAlt;
        case XKB_KEY_Alt_R:     return Key::RAlt;
        default: break;
    }
    if (sym >= XKB_KEY_a && sym <= XKB_KEY_z)
        return static_cast<Key>(static_cast<uint16_t>(Key::A) + (sym - XKB_KEY_a));
    if (sym >= XKB_KEY_A && sym <= XKB_KEY_Z)
        return static_cast<Key>(static_cast<uint16_t>(Key::A) + (sym - XKB_KEY_A));
    if (sym >= XKB_KEY_0 && sym <= XKB_KEY_9)
        return static_cast<Key>(static_cast<uint16_t>(Key::Num0) + (sym - XKB_KEY_0));
    if (sym >= XKB_KEY_F1 && sym <= XKB_KEY_F12)
        return static_cast<Key>(static_cast<uint16_t>(Key::F1) + (sym - XKB_KEY_F1));
    return Key::Unknown;
}
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
        }
        if (state == wl_keyboard::KeyState::RELEASED) {
            std::erase(pressedKeys, key);
            event.key.state = 0;
        }
        uint32_t keySym = xkb.getKeySym(key);
        event.utf8 = xkb.getUTF8(key);
        event.key.keysym = keySym;
        event.key.key = translateKeysym(keySym);
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