#include "wl_buffer.hpp"

#include "opcodes.hpp"
#include "platform/wayland/core/display.hpp"

void Buffer::handleEvent(uint16_t opcode, MessageDeserializer&) {
    if (opcode == static_cast<uint16_t>(wl_buffer::Event::RELEASE)) {
        busy = false;
    }
}

void Buffer::destroy(){
    display.sendRequest(objectId, wl_buffer::Request::DESTROY);
}