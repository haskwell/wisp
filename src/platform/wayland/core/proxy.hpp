#pragma once
#include <cstdint>
#include "../wire/deserializer.hpp"

class Display;

class Proxy{
    protected:
    Display& display;

    public:
    Proxy(Display& d): display(d){}
    uint32_t objectId{};
    virtual void handleEvent(uint16_t opcode, MessageDeserializer& d) = 0;
    virtual ~Proxy();
};