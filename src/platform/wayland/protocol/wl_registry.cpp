#include "wl_registry.hpp"
#include "opcodes.hpp"

const Global* Registry::findGlobal(std::string interface){
    for(const auto& g : globals){
        if(g.interface == interface) return &g;
    }
    return nullptr;
}

void Registry::handleEvent(uint16_t opcode, MessageDeserializer& d) {
    if(opcode == static_cast<uint16_t>(wl_registry::Event::GLOBAL)){
        Global g;
        g.name = d.readUnsignedInteger<uint32_t>();
        //string
        g.interface = d.readString();
        //version
        g.version = d.readUnsignedInteger<uint32_t>();
        globals.push_back(g);
    }
}

std::vector<Global> Registry::getGlobals(){
    return globals;
}