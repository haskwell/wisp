#include "wl_compositor.hpp"
#include "wl_surface.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<Surface> Compositor::createSurface(){
    auto s = std::make_unique<Surface>(display);
    uint32_t id = display.objectManager.registerProxy(s.get());
    display.sendRequest(objectId, wl_compositor::Request::CREATE_SURFACE, id);
    return s;
}