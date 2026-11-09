#include "zxdg_decoration_manager.hpp"
#include "zxdg_toplevel.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

std::unique_ptr<ZxdgTopLevel> ZxdgDecorManager::getZxdgToplevel(uint32_t xdgTopLevelId){
    auto s = display.registerProxy<ZxdgTopLevel>();
    display.sendRequest(objectId, zxdg_decoration_manager_v1::Request::GET_TOPLEVEL_DECORATION, s->objectId, xdgTopLevelId);
    return s;
}
