#pragma once

#include <memory>

#include "wl_keyboard.hpp"
#include "wl_pointer.hpp"
#include "platform/wayland/core/proxy.hpp"

class Seat : public Proxy {
    public:
    uint32_t capabilities{};
    std::unique_ptr<Pointer> pointer;
    std::unique_ptr<Keyboard> keyboard;
    Seat(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t, MessageDeserializer&) override;
    void getPointer();
    void getKeyboard();
};