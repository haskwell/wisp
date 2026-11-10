#include <cstdint>
#include <optional>
#include <vector>

#include "events.hpp"

class EventManager {
public:
    std::vector<Events> events;
    std::optional<Events> searchEvent(uint32_t surfaceId);
    void addEvents(std::vector<Events> &e);
};