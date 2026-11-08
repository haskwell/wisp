#include "../protocol/wl_compositor.hpp"
#include "../protocol/wl_shm.hpp"
#include "../protocol/xdg_wm_base.hpp"
#include "../protocol/zxdg_decoration_manager.hpp"
class Registry;
class Display;
#include <memory>
class Globals{
    std::unique_ptr<Compositor> compositor;
    std::unique_ptr<Shm> shm;
    std::unique_ptr<XdgBase> xdgBase;
    std::unique_ptr<ZxdgDecorManager> zxdgDecorManager;

    Display& display;
    public:
    Globals(Display& display);

    Compositor& getCompositor();
    Shm& getSHM();
    XdgBase& getXdgBase();
    ZxdgDecorManager& getDecorationManager();
};