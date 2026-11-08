#include "xdg_toplevel.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

void XdgTopLevel::setTitle(std::string title){
    display.sendRequest(objectId, xdg_toplevel::Request::SET_TITLE, title);
}

void XdgTopLevel::handleEvent(uint16_t opcode, MessageDeserializer& d){
    if(opcode == static_cast<uint16_t>(xdg_toplevel::Event::CONFIGURE)){
        width = d.readSignedInteger<int32_t>();
        height = d.readSignedInteger<int32_t>();
    }
    if(opcode == static_cast<uint16_t>(xdg_toplevel::Event::CLOSE)){
        display.stop();
    }
}