#include "wl_compositor.hpp"
#include "wl_surface.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<Surface> Compositor::createSurface(){
    auto s = display.registerProxy<Surface>();
    display.sendRequest(objectId, wl_compositor::Request::CREATE_SURFACE, s->objectId);
    return s;
}