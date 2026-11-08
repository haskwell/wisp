#include "zxdg_decoration_manager.hpp"
#include "zxdg_toplevel.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<ZxdgTopLevel> ZxdgDecorManager::getZxdgToplevel(uint32_t xdgTopLevelId){
    auto s = std::make_unique<ZxdgTopLevel>(display);
    uint32_t id = display.objectManager.registerProxy(s.get());
    display.sendRequest(objectId, zxdg_decoration_manager_v1::Request::GET_TOPLEVEL_DECORATION, id, xdgTopLevelId);
    return s;
}
