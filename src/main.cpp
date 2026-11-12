
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
    uint32_t frame = 0;

    while (window.isOpen()) {
        context.dispatch();

        FrameBuffer f = window.getFrameBuffer();

        if (f.pixelData) {
            uint32_t* pixels = f.pixelData;
            uint32_t stridePixels = f.stride / sizeof(uint32_t);

            for (uint32_t y = 0; y < f.height; y++) {
                for (uint32_t x = 0; x < f.width; x++) {
                    uint8_t r = (x * 255 / f.width + frame) % 256;
                    uint8_t g = (y * 255 / f.height + frame) % 256;
                    uint8_t b = 128;

                    pixels[y * stridePixels + x] =
                        (0xFFu << 24) |
                        (static_cast<uint32_t>(r) << 16) |
                        (static_cast<uint32_t>(g) << 8) |
                        b;
                }
            }

            frame++;
        }

        window.update();
    }
}