#include "xdg_surface.hpp"
#include "xdg_toplevel.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<XdgTopLevel> XdgSurface::getToplevel(){
    auto s = display.registerProxy<XdgTopLevel>();
    display.sendRequest(objectId, xdg_surface::Request::GET_TOPLEVEL, s->objectId);
    return s;
}

void XdgSurface::handleEvent(uint16_t opcode, MessageDeserializer& d){
    if(opcode == static_cast<uint16_t>(xdg_surface::Event::CONFIGURE)){
        uint32_t serial = d.readUnsignedInteger<uint32_t>();
        if (onConfigure) onConfigure();
        display.sendRequest(objectId, xdg_surface::Request::ACK_CONFIGURE, serial);
        configured = true;
    }
}

void XdgSurface::destroy() {
    display.sendRequest(objectId, xdg_surface::Request::DESTROY);
}