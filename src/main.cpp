#include "platform/wayland/core/context.hpp"
#include "platform/wayland/core/window.hpp"

#include <iostream>

void handleEvents(Window& window, const char* name) {
    while (auto event = window.pollEvents()) {
        switch (event->type) {
            case EventType::K_ENTER:
                std::cout << name << ": Keyboard focus entered\n";
                break;

            case EventType::K_LEAVE:
                std::cout << name << ": Keyboard focus left\n";
                break;

            case EventType::KEY: {
                const auto& key = event->key;

                std::cout
                    << name
                    << " | Keycode: " << key.keyCode
                    << " | Keysym: " << key.keysym
                    << " | State: " << (key.state ? "PRESSED" : "RELEASED");

                if (key.state) {
                    std::cout << " | UTF-8: \"" << event->utf8 << "\"";
                }

                std::cout << "\nHeld keys: ";

                for (uint32_t k : event->keys) {
                    std::cout << k << " ";
                }

                std::cout << "\n\n";
                break;
            }

            default:
                break;
        }
    }
}

int main() {
    Context context;

    Window window(
        context.getDisplay(), context.getGlobals(),
        800, 600, "Keyboard Test 1"
    );

    Window window1(
        context.getDisplay(), context.getGlobals(),
        800, 600, "Keyboard Test 2"
    );

    window.initialize();
    window1.initialize();

    window.render();
    window1.render();

    while (context.getDisplay().running) {
        context.getDisplay().run();

        handleEvents(window, "Window 1");
        handleEvents(window1, "Window 2");

        window.update();
        window1.update();

        if (window.isClosed() && window1.isClosed()) {
            context.getDisplay().stop();
        }
    }

    return 0;
}