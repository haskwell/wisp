#include "platform/wayland/core/context.hpp"
#include "platform/wayland/core/window.hpp"
#include <iostream>

int main(){

    Context context;
    Window window(context.getDisplay(), context.getGlobals(), 600, 800, "hello");
    window.initialize();
    window.render();
    Window window1(context.getDisplay(), context.getGlobals(), 600, 800, "hello");
    window1.initialize();
    window1.render();
    // Inside your main loop:
    while (context.getDisplay().running) {
        context.getDisplay().run();

        // 1. Poll events for Window 0
        while (auto event = window.pollEvents()) {
            switch (event->type) {
                case EventType::ENTER:
                    std::cout << "[Window 0] Mouse ENTER at ("
                              << event->enter.x << ", " << event->enter.y << ")\n";
                    break;
                case EventType::LEAVE:
                    std::cout << "[Window 0] Mouse LEAVE\n";
                    break;
                case EventType::MOTION:
                    std::cout << "[Window 0] MOTION ("
                              << event->motion.x << ", " << event->motion.y << ")\n";
                    break;
                case EventType::BUTTON:
                    std::cout << "[Window 0] BUTTON " << event->button.button
                              << " state=" << (event->button.state ? "PRESSED" : "RELEASED")
                              << " at (" << event->button.x << ", " << event->button.y << ")\n";
                    break;
                case EventType::AXIS:
                    std::cout << "[Window 0] SCROLL axis=" << event->axis.axis
                              << " value=" << event->axis.value << "\n";
                    break;
            }
        }

        while (auto event = window1.pollEvents()) {
            switch (event->type) {
                case EventType::ENTER:
                    std::cout << "[Window 1] Mouse ENTER at ("
                              << event->enter.x << ", " << event->enter.y << ")\n";
                    break;
                case EventType::LEAVE:
                    std::cout << "[Window 1] Mouse LEAVE\n";
                    break;
                case EventType::MOTION:
                    std::cout << "[Window 1] MOTION ("
                              << event->motion.x << ", " << event->motion.y << ")\n";
                    break;
                case EventType::BUTTON:
                    std::cout << "[Window 1] BUTTON " << event->button.button
                              << " state=" << (event->button.state ? "PRESSED" : "RELEASED")
                              << " at (" << event->button.x << ", " << event->button.y << ")\n";
                    break;
                case EventType::AXIS:
                    std::cout << "[Window 1] SCROLL axis=" << event->axis.axis
                              << " value=" << event->axis.value << "\n";
                    break;
            }
        }
        window.update();
        window1.update();

        if (window.isClosed() && window1.isClosed()) {
            context.getDisplay().stop();
        }
    }

    return 0;
}