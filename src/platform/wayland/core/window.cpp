#include "window.hpp"
#include "display.hpp"
#include "globals.hpp"

Window::Window(Display& d, Globals& g, uint32_t w, uint32_t h, std::string t)
    : display(d), globals(g), width(w), height(h), title(t) {}

void Window::initialize(){
    //create wl surface
    surface = globals.getCompositor().createSurface();
    // xdg surface
    xdgSurface = globals.getXdgBase().getXdgSurface(surface->objectId);
    //xdg toplevel
    xdgTopLevel = xdgSurface->getToplevel();
    xdgTopLevel->setTitle(title);

    zxdgTopLevel = globals.getDecorationManager().getZxdgToplevel(xdgTopLevel->objectId);

    zxdgTopLevel->setModeServer();

    surface->commit();

    while(!xdgSurface->configured){
        display.dispatch();
    }
    
    if(xdgTopLevel->width != 0) width = xdgTopLevel->width;
    if(xdgTopLevel->height != 0) height = xdgTopLevel->height;

    shmBuffer = std::make_unique<ShmBuffer>(globals.getSHM(), width, height);
    shmBuffer->initialize();

    surface->sendAttachRequest(shmBuffer->getBuffer().objectId, 0, 0);
    surface->sendDamageBufferRequest(0, 0, width, height);

    surface->commit();
}