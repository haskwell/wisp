#include "../protocol/wl_registry.hpp"
#include "display.hpp"
#include "globals.hpp"

Globals::Globals(Display& d) : display(d){
    auto& registry = display.getRegistry();
    display.roundTrip();
    compositor = registry.bind<Compositor>("wl_compositor", 4);
    shm = registry.bind<Shm>("wl_shm", 1);
    xdgBase = registry.bind<XdgBase>("xdg_wm_base", 1);
    zxdgDecorManager = registry.bind<ZxdgDecorManager>("zxdg_decoration_manager_v1", 1, false);
    seat = registry.bind<Seat>("wl_seat", 7);
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

ZxdgDecorManager* Globals::getDecorationManager() {
    return zxdgDecorManager.get();
}