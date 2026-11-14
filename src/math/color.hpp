#include <cstdint>

namespace wisp {

    struct Color {
        uint32_t value;

        constexpr Color() : value(0xFF000000u) {}
        constexpr explicit Color(uint32_t v) : value(v) {}
        constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
            : value((uint32_t(a) << 24) | (uint32_t(r) << 16) | (uint32_t(g) << 8) | b) {}

        constexpr uint8_t a() const { return value >> 24; }
        constexpr uint8_t r() const { return value >> 16; }
        constexpr uint8_t g() const { return value >> 8; }
        constexpr uint8_t b() const { return value; }

        static const Color Black;
        static const Color White;
        static const Color Red;
        static const Color Green;
        static const Color Blue;
        static const Color Yellow;
        static const Color Cyan;
        static const Color Magenta;
    };

    inline constexpr Color Color::Black{0, 0, 0};
    inline constexpr Color Color::White{255, 255, 255};
    inline constexpr Color Color::Red{255, 0, 0};
    inline constexpr Color Color::Green{0, 255, 0};
    inline constexpr Color Color::Blue{0, 0, 255};
    inline constexpr Color Color::Yellow{255, 255, 0};
    inline constexpr Color Color::Cyan{0, 255, 255};
    inline constexpr Color Color::Magenta{255, 0, 255};

}