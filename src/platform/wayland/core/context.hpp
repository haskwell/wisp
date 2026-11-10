#pragma once

#include "globals.hpp"
#include "display.hpp"

class Context{
    Display display;
    Globals globals;
    public:
    Context();

    Display& getDisplay();
    Globals& getGlobals();
};