#pragma once
#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include <concepts>
#include <type_traits>
#include <bit>

class MessageDeserializer{
    std::span<const uint8_t> buffer;
    std::size_t cursorPosition = 0;

    public:

    void bytesExist(const std::size_t bytes) const;

    template <std::unsigned_integral T>
    T readBytesUnsigned(std::size_t bytes){
        T res{};
        std::size_t shift = 0;
        std::size_t currentPos = cursorPosition;
        for(; cursorPosition < currentPos + bytes; cursorPosition++, shift++){
            res += (static_cast<T>(buffer[cursorPosition]) << (shift * 8));
        }
        return res;
    }

    template <std::signed_integral T>
    T readBytesSigned(std::size_t bytes){
        T res{};
        std::make_unsigned_t<T> raw{};
        using U = std::make_unsigned_t<T>;
        std::size_t shift = 0;
        std::size_t currentPos = cursorPosition;
        for(; cursorPosition < currentPos + bytes; cursorPosition++, shift++){
            raw += (static_cast<U>(buffer[cursorPosition]) << (shift * 8));
        }
        res = std::bit_cast<T>(raw);
        return res;
    }

    MessageDeserializer(std::span<const uint8_t> b);

    MessageDeserializer();

    void startDeserializer(std::span<const uint8_t> b);

    template <std::unsigned_integral T>
    T readUnsignedInteger(){
        bytesExist(sizeof(T));
        return readBytesUnsigned<T>(sizeof(T));
    }

    template <std::signed_integral T>
    T readSignedInteger(){
        bytesExist(sizeof(T));
        return readBytesSigned<T>(sizeof(T));
    }

    float readFixed();

    uint32_t readObjectID();

    uint16_t readMessageLength();

    uint16_t readOpcode();

    std::string readString();

    std::vector<uint8_t> readArray();
};