#include "wl_pointer.hpp"

#include <iostream>
#include <ostream>

#include "opcodes.hpp"
#include "platform/wayland/core/display.hpp"

void Pointer::handleEvent(uint16_t opcode, MessageDeserializer & d) {
    if (opcode == static_cast<uint16_t>(wl_pointer::Event::ENTER)) {
        serial = d.readUnsignedInteger<uint32_t>();
        surfaceId = d.readUnsignedInteger<uint32_t>();
        insideSurface = true;
        //std::cout << "Surface entered: " << surfaceId << std::endl;
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::LEAVE)) {
        surfaceId = 0;
        insideSurface = false;
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::MOTION)) {
        d.readUnsignedInteger<uint32_t>();
        x = d.readFixed();
        y = d.readFixed();
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::BUTTON)) {
        serial = d.readUnsignedInteger<uint32_t>();
        d.readUnsignedInteger<uint32_t>();
        lastButton = d.readUnsignedInteger<uint32_t>();
        lastButtonState = d.readUnsignedInteger<uint32_t>();
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::AXIS)) {
        d.readUnsignedInteger<uint32_t>();
        uint32_t axis = d.readUnsignedInteger<uint32_t>();
        double value = d.readFixed();
        std::cout << "Axis: " << axis << " Value: " << value << '\n';
        if (axis == static_cast<uint32_t>(wl_pointer::Axis::VERTICAL_SCROLL))
            scrollY += value;

        else if (axis == static_cast<uint32_t>(wl_pointer::Axis::HORIZONTAL_SCROLL))
            scrollX += value;
        //print scroll values
        std::cout << scrollX << std::endl;
        std::cout << scrollY << std::endl;
        std::cout << "===" << std::endl;
    }
}

void Pointer::destroy() {
    display.sendRequest(objectId, wl_pointer::Request::RELEASE);
}
