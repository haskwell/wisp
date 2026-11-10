#include "window.hpp"
#include "display.hpp"
#include "globals.hpp"
#include <algorithm>

#include "platform/wayland/protocol/wl_callback.hpp"

Window::Window(Display& d, Globals& g, uint32_t w, uint32_t h, std::string t)
    : display(d), globals(g), width(w), height(h), title(t) {}

void Window::initialize(){
    //create wl surface
    surface = globals.getCompositor().createSurface();
    // xdg surface
    xdgSurface = globals.getXdgBase().getXdgSurface(surface->objectId);
    xdgSurface->onConfigure = [this](){ handleConfigure(); };
    //xdg toplevel
    xdgTopLevel = xdgSurface->getToplevel();
    xdgTopLevel->setTitle(title);

    if (globals.getDecorationManager()) {
        zxdgTopLevel = globals.getDecorationManager()->getZxdgToplevel(xdgTopLevel->objectId);
        zxdgTopLevel->setModeServer();
    }

    surface->commit();

    while(!xdgSurface->configured){
        display.dispatch();
    }
    
    if(xdgTopLevel->width != 0) width = xdgTopLevel->width;
    if(xdgTopLevel->height != 0) height = xdgTopLevel->height;

    shmBuffer = std::make_unique<ShmBuffer>(globals.getSHM(), width, height);
    shmBuffer->initialize();
}

void Window::present(int bufferIndex) {
    surface->sendAttachRequest(shmBuffer->getBuffer(bufferIndex).objectId, 0, 0);
    surface->sendDamageBufferRequest(0, 0, width, height);
    frameCallback = surface->frame();
    surface->commit();
    shmBuffer->getBuffer(bufferIndex).busy = true;
}

void Window::handleConfigure() {
    if (destroyed || !xdgTopLevel) return;

    uint32_t newWidth = xdgTopLevel->width != 0
        ? xdgTopLevel->width : width;

    uint32_t newHeight = xdgTopLevel->height != 0
        ? xdgTopLevel->height : height;

    if (!shmBuffer) return;
    if (newWidth == width && newHeight == height) return;

    resize(newWidth, newHeight);
}
void Window::resize(uint32_t newWidth, uint32_t newHeight) {
    width = newWidth;
    height = newHeight;
    retiring.push_back(std::move(shmBuffer));
    shmBuffer = std::make_unique<ShmBuffer>(globals.getSHM(), width, height);
    shmBuffer->initialize();
    cleanup();
}

void Window::render() {
    cleanup();
    int bufferIndex = shmBuffer->acquireFreeBuffer();
    if (bufferIndex < 0) return;
    uint32_t* px = shmBuffer->getPixels(bufferIndex);
    std::fill(px, px + static_cast<size_t>(width) * height, frameColor);
    frameColor = nextColor ? 0xFF3366CC : 0xFFCC6633;
    nextColor = !nextColor;

    //std::cout << "Rendering frame " << frameCount
    //          << " using buffer " << bufferIndex << '\n';

    frameCount++;
    present(bufferIndex);
}

bool Window::closeRequested() {
    return xdgTopLevel && xdgTopLevel->closeRequested;
}

bool Window::frameReady() {
    return (!frameCallback || frameCallback->done);
}

void Window::cleanup() {
    for (auto s = retiring.begin(); s != retiring.end(); ) {
        if (!(*s)->anyBusy()) {
            s = retiring.erase(s);
        }
        else s++;
    }
}

std::optional<Events> Window::pollEvents() {
    if (!surface) return std::nullopt;
    return display.checkEvent(surface->objectId);
}

void Window::close() {
    if (destroyed) return;

    destroyed = true;

    if (shmBuffer) {
        retiring.push_back(std::move(shmBuffer));
    }

    if (zxdgTopLevel) {
        zxdgTopLevel->destroy();
        zxdgTopLevel.reset();
    }

    if (xdgTopLevel) {
        xdgTopLevel->destroy();
        xdgTopLevel.reset();
    }

    if (xdgSurface) {
        xdgSurface->destroy();
        xdgSurface.reset();
    }

    if (surface) {
        surface->destroy();
        surface.reset();
    }
}

void Window::update() {
    cleanup();

    if (destroyed) return;

    if (closeRequested()) {
        close();
        return;
    }

    if (frameReady()) {
        render();
    }
}

bool Window::isClosed() const {
    return destroyed && retiring.empty();
}