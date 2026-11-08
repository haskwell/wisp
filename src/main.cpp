#include "platform/wayland/core/context.hpp"
#include "platform/wayland/core/window.hpp"
#include <iostream>

int main(){
    
    Context context;
    Window window(context.getDisplay(), context.getGlobals(), 800, 600, "hello");
    window.initialize();
    
    context.getDisplay().run();
    
    return 0;
}