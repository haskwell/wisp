
#include <wisp/context.hpp>
#include <wisp/window.hpp>
#include <wisp/events.hpp>
#include <iostream>
#include "gfx2d/renderer2d.hpp"
void printEvents(wisp::Window& window, const char* name) {
    while (auto event = window.pollEvents()) {
        std::cout << "\n[" << name << "] ";

        std::cout << "Surface: " << event->surfaceID
                  << " | Timestamp: " << event->timestamp
                  << " | ";

        switch (event->type) {
            case wisp::EventType::P_ENTER:
                std::cout << "POINTER ENTER"
                          << " | Serial: " << event->enter.serial
                          << " | Position: (" << event->enter.x
                          << ", " << event->enter.y << ")";
                break;

            case wisp::EventType::P_LEAVE:
                std::cout << "POINTER LEAVE"
                          << " | Serial: " << event->leave.serial;
                break;

            case wisp::EventType::MOTION:
                std::cout << "POINTER MOTION"
                          << " | Position: (" << event->motion.x
                          << ", " << event->motion.y << ")";
                break;

            case wisp::EventType::BUTTON:
                std::cout << "POINTER BUTTON"
                          << " | Serial: " << event->button.serial
                          << " | Button: " << event->button.button
                          << " | State: " << event->button.state
                          << " | Position: (" << event->button.x
                          << ", " << event->button.y << ")";
                break;

            case wisp::EventType::AXIS:
                std::cout << "POINTER AXIS"
                          << " | Axis: " << event->axis.axis
                          << " | Value: " << event->axis.value;
                break;

            case wisp::EventType::K_ENTER:
                std::cout << "KEYBOARD ENTER"
                          << " | Serial: " << event->k_enter.serial
                          << " | Keys: ";

                for (auto key : event->keys)
                    std::cout << key << " ";

                break;

            case wisp::EventType::K_LEAVE:
                std::cout << "KEYBOARD LEAVE"
                          << " | Serial: " << event->k_leave.serial;
                break;

            case wisp::EventType::KEY:
                std::cout << "KEY"
                          << " | Serial: " << event->key.serial
                          << " | Keycode: " << event->key.keyCode
                          << " | Keysym: " << event->key.keysym
                          << " | State: " << event->key.state
                          << " | UTF-8: " << event->utf8;
                break;
        }

        std::cout << '\n';
    }
}

int main() {
    wisp::Context context;
    wisp::Window window(context, 800, 600, "wisp");
    wisp::gfx2d::Renderer renderer(window);

    Vec2 point(100, 100);
    while (window.isOpen()) {
        context.dispatch();
        renderer.beginFrame();

        renderer.drawPoint(point, wisp::Color::White);


        renderer.endFrame();
    }
}