#include "platform/wayland/core/display.hpp"
#include "platform/wayland/protocol/wl_registry.hpp"
#include "platform/wayland/protocol/wl_compositor.hpp"
#include "platform/wayland/protocol/xdg_wm_base.hpp"
#include "platform/wayland/protocol/wl_shm.hpp"
#include "platform/wayland/protocol/zxdg_decoration_manager.hpp"
#include <iostream>

int main(){
    Display display;
    auto registry = display.getRegistry();
    display.roundTrip();

    for (auto& g : registry->getGlobals())
    std::cout << g.name << " " << g.interface << " v" << g.version << '\n';

    auto compositor = registry->bind<Compositor>("wl_compositor");
    auto shm = registry->bind<SHM>("wl_shm");
    auto xdgbase = registry->bind<XdgBase>("xdg_wm_base");
    auto zxdgdecorman = registry->bind<ZxdgDecorManager>("zxdg_decoration_manager_v1");

    display.roundTrip();

    return 0;
}