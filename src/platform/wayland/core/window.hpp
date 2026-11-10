#pragma once

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
    void close();


public:
    Window(Display& d, Globals& g, uint32_t width, uint32_t height, std::string title);
    void initialize();
    void present(int bufferIndex);
    void update();

    bool isClosed() const;

    void handleConfigure();

    void resize(uint32_t newWidth, uint32_t newHeight);

    void render();

    bool closeRequested();

    bool frameReady();
    void cleanup();
};