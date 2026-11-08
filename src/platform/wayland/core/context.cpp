#include "context.hpp"

Context::Context() : display(), globals(display) {}

Display& Context::getDisplay(){
    return display;
}
Globals& Context::getGlobals(){
    return globals;
}