#include "xkb.hpp"

#include <stdexcept>

#include "platform/wayland/protocol/opcodes.hpp"

XKB::XKB() {
    context = xkb_context_new(XKB_CONTEXT_NO_FLAGS);
    if (!context) {
        throw std::runtime_error("failed to create xkb context");
    }
}

void XKB::initialize(const char* str) {
    if (state) {xkb_state_unref(state); state = nullptr;}
    if (keymap) {xkb_keymap_unref(keymap); keymap = nullptr;}

    keymap = xkb_keymap_new_from_string(context, str, XKB_KEYMAP_FORMAT_TEXT_V1, XKB_KEYMAP_COMPILE_NO_FLAGS);
    if (!keymap) {
        throw std::runtime_error("failed to create xkb keymap");
    }

    state = xkb_state_new(keymap);
    if (!state) {
        throw std::runtime_error("failed to create xkb state");
    }
}

void XKB::updateModifierState(uint32_t depressed, uint32_t latched, uint32_t locked, uint32_t group) {
    xkb_state_update_mask(state, depressed, latched, locked, 0, 0, group);
}

uint32_t XKB::getKeySym(uint32_t key) {
    return xkb_state_key_get_one_sym(state, key + 8);
}

std::string XKB::getUTF8(uint32_t key) {
    char buffer[10];
    int length = xkb_state_key_get_utf8(state, key + 8, buffer, sizeof(buffer));
    std::string result(buffer);
    return result;
}

XKB::~XKB() {
    if (state) xkb_state_unref(state);
    if (keymap) xkb_keymap_unref(keymap);
    if (context) xkb_context_unref(context);
}
