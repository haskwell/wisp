#pragma once
#include <cstdint>
#include <memory>
#include <string>
#include <optional>
#include <wisp/events.hpp>
namespace wisp {
    class Context;
    class Window {
        struct Implementation;
        std::unique_ptr<Implementation> impl;
    public:
        ~Window();
        Window(Context& c, uint32_t width, uint32_t height, std::string title);
        void update();
        bool isOpen() const;
        std::optional<Events> pollEvents();
    };
}