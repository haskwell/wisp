#include <stdexcept>

#include "deserializer.hpp"

void MessageDeserializer::bytesExist(const std::size_t bytes) const{
    if((cursorPosition <= buffer.size()) && (bytes <= (buffer.size() - cursorPosition))){
        return;
    }
    else{
        throw std::runtime_error("not enough bytes available to read");
    }
}


MessageDeserializer::MessageDeserializer(std::span<const uint8_t> b){
    buffer = b;
}

MessageDeserializer::MessageDeserializer(){}

void MessageDeserializer::startDeserializer(std::span<const uint8_t> b){
    buffer = b;
    cursorPosition = 0;
}

float MessageDeserializer::readFixed(){
    bytesExist(4);
    int32_t res = readSignedInteger<int32_t>();
    return res/256.f;
}

uint32_t MessageDeserializer::readObjectID(){
    bytesExist(4);
    return readBytesUnsigned<uint32_t>(4);
}

uint16_t MessageDeserializer::readMessageLength(){
    bytesExist(2);
    return readBytesUnsigned<uint16_t>(2);
}

uint16_t MessageDeserializer::readOpcode(){
    bytesExist(2);
    return readBytesUnsigned<uint16_t>(2);
}

std::string MessageDeserializer::readString(){
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

std::vector<uint8_t> MessageDeserializer::readArray(){
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

std::vector<uint32_t> MessageDeserializer::readArray32bit() {
    uint32_t arrLen{};
    bytesExist(4);
    arrLen = readBytesUnsigned<uint32_t>(4);
    std::size_t bytesToRead = arrLen;
    if (arrLen % 4 != 0) {
        throw std::runtime_error("Invalid 32-bit array length");
    }
    bytesExist(bytesToRead);
    std::size_t currentPos = cursorPosition;
    std::vector<uint8_t> arr(bytesToRead);
    for(std::size_t i = 0;cursorPosition < currentPos + bytesToRead;i++, cursorPosition++){
        arr[i] = buffer[cursorPosition];
    }
    std::size_t padding = (4 - (arrLen % 4)) % 4;
    bytesExist(padding);
    cursorPosition += padding;

    std::vector<uint32_t> res;
    for (size_t i = 0; i < arr.size(); i += 4) {
        uint32_t element =
            static_cast<uint32_t>(arr[i]) |
                static_cast<uint32_t>(arr[i + 1]) << 8 |
                    static_cast<uint32_t>(arr[i + 2]) << 16 |
                        static_cast<uint32_t>(arr[i + 3]) << 24;
        res.push_back(element);
    }
    return res;
}
