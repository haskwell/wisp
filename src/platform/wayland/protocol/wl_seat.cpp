
#include "wl_seat.hpp"
#include "../core/display.hpp"
#include <iostream>
#include <ostream>

#include "opcodes.hpp"

void Seat::handleEvent(uint16_t opcode, MessageDeserializer & d) {
    if (opcode == static_cast<uint16_t>(wl_seat::Event::CAPABILITIES)) {
        capabilities = d.readUnsignedInteger<uint32_t>();
        if (capabilities & wl_seat::Capability::KEYBOARD) {
            if (!keyboard) getKeyboard();
        }
        if (!(capabilities & wl_seat::Capability::KEYBOARD)) {
            if (keyboard) {
                keyboard->destroy();
                keyboard.reset();
            }
        }
        if (capabilities & wl_seat::Capability::POINTER) {
            if (!pointer) getPointer();
        }
        if (!(capabilities & wl_seat::Capability::POINTER)) {
            if (pointer) {
                pointer->destroy();
                pointer.reset();
            }
        }
        if (capabilities & wl_seat::Capability::TOUCH) {
            std::cout << "touch" << std::endl;
        }
    }
}

void Seat::getPointer() {
    pointer = display.registerProxy<Pointer>();
    display.sendRequest(objectId, wl_seat::Request::GET_POINTER, pointer->objectId);
}

void Seat::getKeyboard() {
    keyboard = display.registerProxy<Keyboard>();
    display.sendRequest(objectId, wl_seat::Request::GET_KEYBOARD, keyboard->objectId);
}
