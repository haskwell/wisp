#include "xdg_surface.hpp"
#include "xdg_toplevel.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<XdgTopLevel> XdgSurface::getToplevel(){
    auto s = std::make_unique<XdgTopLevel>(display);
    uint32_t id = display.objectManager.registerProxy(s.get());
    display.sendRequest(objectId, xdg_surface::Request::GET_TOPLEVEL, id);
    return s;
}

void XdgSurface::handleEvent(uint16_t opcode, MessageDeserializer& d){
    if(opcode == static_cast<uint16_t>(xdg_surface::Event::CONFIGURE)){
        uint32_t serial = d.readUnsignedInteger<uint32_t>();
        display.sendRequest(objectId, xdg_surface::Request::ACK_CONFIGURE, serial);
        configured = true;
    }
}