#pragma once
#include <cstdint>
#include <optional>
#include <vector>
#include <wisp/events.hpp>

class EventManager {
public:
    std::vector<wisp::Events> events;
    std::optional<wisp::Events> searchEvent(uint32_t surfaceId);
    void addEvents(std::vector<wisp::Events> &e);

    void addEvent(wisp::Events &e);
};