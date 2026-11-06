#pragma once
#include <cstdint>
#include <span>
#include <string>
#include <vector>
#include <stdexcept>
#include <concepts>

class MessageDeserializer{
    std::span<const uint8_t> buffer;
    std::size_t cursorPosition = 0;

    public:

    void bytesExist(std::size_t bytes) const{
        if((cursorPosition <= buffer.size()) && (bytes <= (buffer.size() - cursorPosition))){
            return;
        }
        else{
            throw std::runtime_error("not enough bytes available to read");
        }
    }

    template <std::unsigned_integral T>
    T readBytes(std::size_t bytes){
        T res{};
        std::size_t shift = 0;
        std::size_t currentPos = cursorPosition;
        for(; cursorPosition < currentPos + bytes; cursorPosition++, shift++){
            res += (static_cast<T>(buffer[cursorPosition]) << (shift * 8));
        }
        return res;
    }

    MessageDeserializer(std::span<const uint8_t> b){
        buffer = b;
    }

    template <std::unsigned_integral T>
    T readInteger(){
        bytesExist(sizeof(T));
        return readBytes<T>(sizeof(T));
    }

    uint32_t readObjectID(){
        bytesExist(4);
        return readBytes<uint32_t>(4);
    }

    uint16_t readMessageLength(){
        bytesExist(2);
        return readBytes<uint16_t>(2);
    }

    uint16_t readOpcode(){
        bytesExist(2);
        return readBytes<uint16_t>(2);
    }

    std::string readString(){
        uint32_t strLen{};
        bytesExist(4);
        strLen = readBytes<uint32_t>(4);
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
        arrLen = readBytes<uint32_t>(4);
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