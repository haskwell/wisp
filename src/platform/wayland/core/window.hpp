#include "../protocol/wl_surface.hpp"
#include "../protocol/xdg_surface.hpp"
#include "../protocol/xdg_toplevel.hpp"
#include "../protocol/zxdg_toplevel.hpp"
#include "shmBuffer.hpp"
#include <string>
#include <memory>
class Display;
class Globals;
class Window {
private:
    Display& display;
    Globals& globals;

    uint32_t width;
    uint32_t height;
    std::string title;
    std::unique_ptr<Surface> surface;
    std::unique_ptr<XdgSurface> xdgSurface;
    std::unique_ptr<XdgTopLevel> xdgTopLevel;
    std::unique_ptr<ZxdgTopLevel> zxdgTopLevel;
    std::unique_ptr<ShmBuffer> shmBuffer;
    public:
    Window(Display& d, Globals& g, uint32_t width, uint32_t height, std::string title);
    void initialize();
};