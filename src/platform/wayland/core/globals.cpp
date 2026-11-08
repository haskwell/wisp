#include "../protocol/wl_registry.hpp"
#include "display.hpp"
#include "globals.hpp"

Globals::Globals(Display& d) : display(d){
    auto& registry = display.getRegistry();
    display.roundTrip();
    compositor = registry.bind<Compositor>("wl_compositor");
    shm = registry.bind<Shm>("wl_shm");
    xdgBase = registry.bind<XdgBase>("xdg_wm_base");
    zxdgDecorManager = registry.bind<ZxdgDecorManager>("zxdg_decoration_manager_v1");
}

Compositor& Globals::getCompositor() {
    return *compositor;
}

Shm& Globals::getSHM() {
    return *shm;
}

XdgBase& Globals::getXdgBase() {
    return *xdgBase;
}

ZxdgDecorManager& Globals::getDecorationManager() {
    return *zxdgDecorManager;
}