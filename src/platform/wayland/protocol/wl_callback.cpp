#include "wl_callback.hpp"
#include "opcodes.hpp"

void Callback::handleEvent(uint16_t opcode, MessageDeserializer& d) {
    if(opcode == static_cast<uint16_t>(wl_callback::Event::DONE)) done = true;
}