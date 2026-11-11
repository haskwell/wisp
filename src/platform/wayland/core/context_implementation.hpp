#pragma once
#include "display.hpp"
#include "globals.hpp"

namespace wisp {
    struct Context::Implementation {
        Display display;
        Globals globals;
    public:
        Implementation() : globals(display) {}
    };
}