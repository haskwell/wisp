#include "event_manager.hpp"

#include <cstdint>

std::optional<Events> EventManager::searchEvent(uint32_t surfaceId) {
    for (auto it = events.begin(); it != events.end(); ++it) {
        if (it->surfaceID == surfaceId) {
            Events event = *it;
            events.erase(it);
            return event;
        }
    }
    return std::nullopt;
}

void EventManager::addEvents(std::vector<Events>& e) {
    events.insert(events.end(), e.begin(), e.end());
}
