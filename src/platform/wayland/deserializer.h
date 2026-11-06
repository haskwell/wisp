#pragma once
#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include <stdexcept>
#include <concepts>
#include <type_traits>
#include <bit>

class MessageDeserializer{
    std::span<const uint8_t> buffer;
    std::size_t cursorPosition = 0;

    public:

    void bytesExist(const std::size_t bytes) const{
        if((cursorPosition <= buffer.size()) && (bytes <= (buffer.size() - cursorPosition))){
            return;
        }
        else{
            throw std::runtime_error("not enough bytes available to read");
        }
    }

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

    MessageDeserializer(std::span<const uint8_t> b){
        buffer = b;
    }

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

    float readFixed(){
        bytesExist(4);
        int32_t res = readSignedInteger<int32_t>();
        return res/256.f;
    }

    uint32_t readObjectID(){
        bytesExist(4);
        return readBytesUnsigned<uint32_t>(4);
    }

    uint16_t readMessageLength(){
        bytesExist(2);
        return readBytesUnsigned<uint16_t>(2);
    }

    uint16_t readOpcode(){
        bytesExist(2);
        return readBytesUnsigned<uint16_t>(2);
    }

    std::string readString(){
        uint32_t strLen{};
        bytesExist(4);
        strLen = readBytesUnsigned<uint32_t>(4);
        std::string str{};
        if (strLen == 0) {
            throw std::runtime_error("invalid string");
        }
        std::size_t padding = (4 - (strLen % 4)) % 4;
        bytesExist(strLen + padding);
        std::size_t bytesToRead = strLen - 1;
        std::size_t currentPos = cursorPosition;
        for(;cursorPosition < currentPos + bytesToRead; cursorPosition++){
            str += buffer[cursorPosition];
        }
        if (buffer[cursorPosition] != '\0'){
            throw std::runtime_error("invalid string");
        }
        cursorPosition++; // for '\0'
        cursorPosition += padding;
        return str;
    }

    std::vector<uint8_t> readArray(){
        uint32_t arrLen{};
        bytesExist(4);
        arrLen = readBytesUnsigned<uint32_t>(4);
        std::size_t bytesToRead = arrLen;
        bytesExist(bytesToRead);
        std::size_t currentPos = cursorPosition;
        std::vector<uint8_t> arr(bytesToRead);
        for(std::size_t i = 0;cursorPosition < currentPos + bytesToRead;i++, cursorPosition++){
            arr[i] = buffer[cursorPosition];
        }
        std::size_t padding = (4 - (arrLen % 4)) % 4;
        bytesExist(padding);
        cursorPosition += padding;
        return arr;
    }
};