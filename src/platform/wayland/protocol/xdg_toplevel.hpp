#pragma once
#include "../core/proxy.hpp"
#include <string>
#include <cstdint>

class XdgTopLevel : public Proxy {
    public:
    int32_t width{0};
    int32_t height{0};
    bool closeRequested = false;
    XdgTopLevel(Display& d) : Proxy(d) {}
    void handleEvent(uint16_t opcode, MessageDeserializer& d) override;

    void destroy();

    void setTitle(std::string title);
};