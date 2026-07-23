#pragma once

#include "contracts.h"
#include <functional>
#include <vector>
#include <map>
#include <chrono>

namespace ai {

using EventHandler = std::function<void(const Event&)>;

class EventBus {
private:
    std::map<EventType, std::vector<EventHandler>> listeners_;

public:
    void Subscribe(EventType type, EventHandler handler) {
        listeners_[type].push_back(handler);
    }

    void Publish(EventType type, const std::wstring& payload) {
        double timestamp = std::chrono::duration<double>(
            std::chrono::system_clock::now().time_since_epoch()
        ).count();
        
        Event event{ type, payload, timestamp };
        if (listeners_.find(type) != listeners_.end()) {
            for (const auto& handler : listeners_[type]) {
                handler(event);
            }
        }
    }
};

} // namespace ai
