#include <wisp/context.hpp>
#include <wisp/window.hpp>
int main() {
    wisp::Context context;
    wisp::Window window(context, 800, 600, "wisp");
    wisp::Window window1(context, 800, 600, "wisp");

    while (window.isOpen() || window1.isOpen()) {
        context.dispatch();
        if (window.isOpen()) window.update();
        if (window1.isOpen()) window1.update();
        window.pollEvents();
    }
}