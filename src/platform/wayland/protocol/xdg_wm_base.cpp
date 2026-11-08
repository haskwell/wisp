#include "xdg_wm_base.hpp"
#include "xdg_surface.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<XdgSurface> XdgBase::getXdgSurface(uint32_t surfaceId){
    auto s = std::make_unique<XdgSurface>(display);
    uint32_t id = display.objectManager.registerProxy(s.get());
    display.sendRequest(objectId, xdg_wm_base::Request::GET_XDG_SURFACE, id, surfaceId);
    return s;
}

void XdgBase::handleEvent(uint16_t opcode, MessageDeserializer& d){
    if(opcode == static_cast<uint16_t>(xdg_wm_base::Event::PING)){
        uint32_t ping = d.readUnsignedInteger<uint32_t>();
        display.sendRequest(objectId, xdg_wm_base::Request::PONG, ping);
    }
}