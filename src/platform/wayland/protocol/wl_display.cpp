#include "wl_display.hpp"
#include "wl_registry.hpp"
#include "wl_callback.hpp"
#include "../core/display.hpp"
#include "opcodes.hpp"
#include <stdexcept>
#include <string>

void WlDisplay::handleEvent(uint16_t opcode, MessageDeserializer& d){
    if(opcode == static_cast<uint16_t>(wl_display::Event::ERROR)){
        uint32_t id = d.readUnsignedInteger<uint32_t>();
        uint32_t code = d.readUnsignedInteger<uint32_t>();
        std::string msg = d.readString();
        throw std::runtime_error("wl_display.error: object " + std::to_string(id) +
                                 " code " + std::to_string(code) + ": " + msg);
    }
    if(opcode == static_cast<uint16_t>(wl_display::Event::DELETE_ID)){
        //delete object
    }
}

std::unique_ptr<Registry> WlDisplay::getRegistry(){
    auto registry = std::make_unique<Registry>(display);
    display.objectManager.registerProxy(registry.get());
    display.sendRequest(objectId, wl_display::Request::GET_REGISTRY, registry->objectId);
    return registry;
}

std::unique_ptr<Callback> WlDisplay::sync(){
    auto callback = std::make_unique<Callback>(display);
    display.objectManager.registerProxy(callback.get());
    display.sendRequest(objectId, wl_display::Request::SYNC, callback->objectId);
    return callback;
}