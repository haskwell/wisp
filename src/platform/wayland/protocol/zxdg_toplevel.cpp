#include "zxdg_toplevel.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

void ZxdgTopLevel::setModeServer(){
    display.sendRequest(objectId, zxdg_toplevel_decoration_v1::Request::SET_MODE, static_cast<uint32_t>(zxdg_toplevel_decoration_v1::Mode::SERVER_SIDE));
}

void ZxdgTopLevel::handleEvent(uint16_t opcode, MessageDeserializer& d) {
    if(opcode == static_cast<uint16_t>(zxdg_toplevel_decoration_v1::Event::CONFIGURE)){
        d.readUnsignedInteger<uint32_t>();
    }
}

void ZxdgTopLevel::destroy() {
    display.sendRequest(objectId, zxdg_toplevel_decoration_v1::Request::DESTROY);
}