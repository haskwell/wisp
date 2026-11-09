#include "wl_surface.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"

void Surface::commit(){
    display.sendRequest(objectId, wl_surface::Request::COMMIT);
}

std::unique_ptr<Callback> Surface::frame() {
    auto callback = display.registerProxy<Callback>();
    display.sendRequest(objectId, wl_surface::Request::FRAME, callback->objectId);
    return callback;
}

void Surface::sendAttachRequest(uint32_t bufferId, int32_t x, int32_t y){
    display.sendRequest(objectId, wl_surface::Request::ATTACH, bufferId, x, y);
}
void Surface::sendDamageBufferRequest(int32_t x, int32_t y, int32_t width, int32_t height){
    display.sendRequest(objectId, wl_surface::Request::DAMAGE_BUFFER, x, y, width, height);
}
void Surface::destroy() {
    display.sendRequest(objectId, wl_surface::Request::DESTROY);
}