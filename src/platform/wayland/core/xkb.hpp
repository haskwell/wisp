#pragma once
#include <string>
#include <xkbcommon/xkbcommon.h>

#include "platform/wayland/protocol/opcodes.hpp"

class XKB {
    xkb_context* context = nullptr;
    xkb_keymap* keymap = nullptr;
    xkb_state* state = nullptr;
public:
    XKB();

    void initialize(const char* str);
    void updateModifierState(uint32_t depressed,
        uint32_t latched,
        uint32_t locked,
        uint32_t group);
    uint32_t getKeySym(uint32_t key);
    std::string getUTF8(uint32_t key);
    ~XKB();
};
