#include <iostream>
#include <iomanip>
#include "platform/wayland/connection.h"
#include "platform/wayland/serializer.h"
#include "platform/wayland/deserializer.h"
int main(){
    Connection c;
    MessageSerializer s;
    s.beginMessage(1, 0);
    s.append_uint(uint32_t{2});
    s.finishMessage();

    c.sendMessage(s.getMessage());

    std::vector<uint8_t> res = c.receiveMessage();

    std::span<uint8_t> res1 = res;

    MessageDeserializer d(res1);

    uint32_t objectID = d.readObjectID();
    uint16_t opcode = d.readOpcode();
    uint16_t messageLen = d.readMessageLength();
    uint32_t timestamp = d.readInteger<uint32_t>();

    std::cout << objectID << '\n' << opcode << '\n' << messageLen << '\n' << timestamp << '\n';

    return 0;
}