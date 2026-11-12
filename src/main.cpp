
#include <algorithm>
#include <wisp/context.hpp>
#include <wisp/window.hpp>
#include <wisp/events.hpp>
#include <wisp/frameBuffer.hpp>
#include <iostream>
#include <cstdint>
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

    bool left = false, right = false, up = false, down = false;
    int offsetX = 0, offsetY = 0;   // square's offset from the window center
    const int speed = 6;            // pixels per presented frame

    while (window.isOpen()) {
        context.dispatch();

        // drain pending events and track which movement keys are held
        while (auto event = window.pollEvents()) {
            switch (event->type) {
                case wisp::EventType::KEY: {
                    bool pressed = (event->key.state == 1);
                    switch (event->key.key) {
                        case wisp::Key::Left:
                        case wisp::Key::A:     left  = pressed; break;
                        case wisp::Key::Right:
                        case wisp::Key::D:     right = pressed; break;
                        case wisp::Key::Up:
                        case wisp::Key::W:     up    = pressed; break;
                        case wisp::Key::Down:
                        case wisp::Key::S:     down  = pressed; break;
                        default: break;
                    }
                    break;
                }
                case wisp::EventType::K_LEAVE:
                    // lost keyboard focus: release everything so the square doesn't drift
                    left = right = up = down = false;
                    break;
                default:
                    break;
            }
        }

        FrameBuffer f = window.getFrameBuffer();

        if (f.pixelData) {
            uint32_t* pixels = f.pixelData;
            size_t stridePixels = f.stride / sizeof(uint32_t);

            const uint32_t background  = 0xFF202020;  // dark gray
            const uint32_t squareColor = 0xFFFF8C00;  // orange

            int w = static_cast<int>(f.width);
            int h = static_cast<int>(f.height);
            int side = std::min(w, h) / 3;

            // move, then clamp so the square stays fully inside the window
            offsetX += (right - left) * speed;
            offsetY += (down - up) * speed;
            int maxX = (w - side) / 2;
            int maxY = (h - side) / 2;
            offsetX = std::clamp(offsetX, -maxX, maxX);
            offsetY = std::clamp(offsetY, -maxY, maxY);

            int x0 = (w - side) / 2 + offsetX;
            int y0 = (h - side) / 2 + offsetY;

            for (int y = 0; y < h; y++) {
                for (int x = 0; x < w; x++) {
                    bool inSquare = x >= x0 && x < x0 + side &&
                                    y >= y0 && y < y0 + side;
                    pixels[y * stridePixels + x] = inSquare ? squareColor : background;
                }
            }
        }

        window.update();
    }
}