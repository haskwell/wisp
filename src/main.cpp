#include "platform/wayland/core/context.hpp"
#include "platform/wayland/core/window.hpp"

int main(){
    
    Context context;
    Window window(context.getDisplay(), context.getGlobals(), 800, 600, "hello");
    window.initialize();
    window.render();

    Window window1(context.getDisplay(), context.getGlobals(), 800, 600, "hello");
    window1.initialize();
    window1.render();
    while (context.getDisplay().running) {
        context.getDisplay().run();

        window.update();
        window1.update();

        if (window.isClosed() && window1.isClosed()) {
            context.getDisplay().stop();
        }
    }
    
    return 0;
}