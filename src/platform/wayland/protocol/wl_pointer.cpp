#include "wl_pointer.hpp"

#include "opcodes.hpp"
#include "platform/wayland/core/display.hpp"
#include <utility>
using namespace wisp;
void Pointer::handleEvent(uint16_t opcode, MessageDeserializer & d) {
    if (opcode == static_cast<uint16_t>(wl_pointer::Event::ENTER)) {
        Events event{};
        event.type = EventType::P_ENTER;
        event.enter.serial = d.readUnsignedInteger<uint32_t>();
        event.surfaceID = d.readUnsignedInteger<uint32_t>();
        event.enter.x = d.readFixed();
        event.enter.y = d.readFixed();

        currentSurfaceID = event.surfaceID;
        position.first = event.enter.x;
        position.second = event.enter.y;

        pendingFrame.push_back(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::LEAVE)) {
        Events event{};
        event.type = EventType::P_LEAVE;
        event.leave.serial = d.readUnsignedInteger<uint32_t>();
        event.surfaceID = d.readUnsignedInteger<uint32_t>();
        pendingFrame.push_back(event);
        currentSurfaceID = 0;
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::MOTION)) {
        Events event{};
        event.type = EventType::MOTION;
        event.timestamp = d.readUnsignedInteger<uint32_t>();
        event.motion.x = d.readFixed();
        event.motion.y = d.readFixed();
        position = {event.motion.x, event.motion.y};
        event.surfaceID = currentSurfaceID;
        pendingFrame.push_back(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::BUTTON)) {
        Events event{};
        event.type = EventType::BUTTON;
        event.button.serial = d.readUnsignedInteger<uint32_t>();
        event.timestamp = d.readUnsignedInteger<uint32_t>();
        event.button.button = d.readUnsignedInteger<uint32_t>();
        event.button.state = d.readUnsignedInteger<uint32_t>();

        event.button.x = position.first;
        event.button.y = position.second;
        event.surfaceID = currentSurfaceID;

        pendingFrame.push_back(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::AXIS)) {
        Events event{};
        event.type = EventType::AXIS;
        event.timestamp = d.readUnsignedInteger<uint32_t>();
        event.axis.axis = d.readUnsignedInteger<uint32_t>();
        event.axis.value = d.readFixed();

        event.surfaceID = currentSurfaceID;
        pendingFrame.push_back(event);
    }
    else if (opcode == static_cast<uint16_t>(wl_pointer::Event::FRAME)) {
        flushFrame();
    }
}

void Pointer::flushFrame() {
    if (pendingFrame.empty()) return;
    display.addEvents(pendingFrame);
    pendingFrame.clear();
}

void Pointer::destroy() {
    display.sendRequest(objectId, wl_pointer::Request::RELEASE);
}
