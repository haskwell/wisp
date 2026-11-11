#include "display.hpp"
#include "globals.hpp"
#include <algorithm>
#include <wisp/window.hpp>
#include <wisp/context.hpp>
#include "../protocol/wl_callback.hpp"
#include "../protocol/wl_surface.hpp"
#include "../protocol/xdg_surface.hpp"
#include "../protocol/xdg_toplevel.hpp"
#include "../protocol/zxdg_toplevel.hpp"
#include "shmBuffer.hpp"
#include "context_implementation.hpp"
using namespace wisp;
struct Window::Implementation {
    Display& display;
    Globals& globals;
    uint32_t frameColor = 0xFF3366CC;
    bool nextColor = false;
    uint32_t width;
    uint32_t height;
    std::string title;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<XdgSurface> xdgSurface;
    std::unique_ptr<XdgTopLevel> xdgTopLevel;
    std::unique_ptr<ZxdgTopLevel> zxdgTopLevel;
    std::unique_ptr<ShmBuffer> shmBuffer;
    std::unique_ptr<Callback> frameCallback;
    uint32_t frameCount = 0;
    std::vector<std::unique_ptr<ShmBuffer>> retiring;
    bool destroyed = false;
public:
    Implementation(Display& d, Globals& g, uint32_t w, uint32_t h, std::string t) :
        display(d), globals(g), width(w), height(h), title(t) {}
    void initialize(){
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
        render();
    }
    void present(int bufferIndex) {
        surface->sendAttachRequest(shmBuffer->getBuffer(bufferIndex).objectId, 0, 0);
        surface->sendDamageBufferRequest(0, 0, width, height);
        frameCallback = surface->frame();
        surface->commit();
        shmBuffer->getBuffer(bufferIndex).busy = true;
    }
    void handleConfigure() {
        if (destroyed || !xdgTopLevel) return;

        uint32_t newWidth = xdgTopLevel->width != 0
            ? xdgTopLevel->width : width;

        uint32_t newHeight = xdgTopLevel->height != 0
            ? xdgTopLevel->height : height;

        if (!shmBuffer) return;
        if (newWidth == width && newHeight == height) return;

        resize(newWidth, newHeight);
    }
    void resize(uint32_t newWidth, uint32_t newHeight) {
        width = newWidth;
        height = newHeight;
        retiring.push_back(std::move(shmBuffer));
        shmBuffer = std::make_unique<ShmBuffer>(globals.getSHM(), width, height);
        shmBuffer->initialize();
        cleanup();
    }
    void render() {
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
    bool closeRequested() const {
        return xdgTopLevel && xdgTopLevel->closeRequested;
    }
    bool frameReady() const {
        return (!frameCallback || frameCallback->done);
    }
    void cleanup() {
        for (auto s = retiring.begin(); s != retiring.end(); ) {
            if (!(*s)->anyBusy()) {
                s = retiring.erase(s);
            }
            else s++;
        }
    }
    void close() {
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
    void update() {
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
    bool isClosed() const {
        return destroyed && retiring.empty();
    }
};

Window::Window(Context& c, uint32_t width, uint32_t height, std::string title) {
    impl = std::make_unique<Implementation>(
        c.implementation->display,
        c.implementation->globals, width, height, title);
    impl->initialize();
}
Window::~Window() = default;

bool Window::isOpen() const {
    return !(impl->isClosed());
}
void Window::update() {
    impl->update();
}

std::optional<Events> Window::pollEvents() {
    if (!impl->surface) return std::nullopt;
    return impl->display.checkEvent(impl->surface->objectId);
}


