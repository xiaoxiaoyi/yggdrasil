#ifndef _YGG_EVENTMANAGER_HPP
#define _YGG_EVENTMANAGER_HPP

#include "Manager.hpp"
#include "core/StringID.hpp"

#include <unordered_map>

namespace ygg {

class Event {
public:
    virtual ~Event() {
    }

    virtual StringID getType() const = 0;
};

typedef std::function<void (const Event&)> EventListener;
typedef uint32_t ListenerID;

class EventManager: public Manager {
    typedef std::vector<EventListener> ListenerContainer;
    typedef std::unordered_map<StringID, ListenerContainer> ListenerMap;
public:
    static StringID getTypeName() {
        static StringID name("EventManager");
        return name;
    }

    ListenerID addEventListener(StringID eventType, EventListener listener) {
        ListenerContainer& container = m_Listeners[eventType];
        container.emplace_back(listener);
        return container.size() - 1;
    }

    void removeEventListener(StringID eventType, ListenerID listenerID) {
        ListenerContainer& container = m_Listeners[eventType];
        std::swap(container[listenerID], container.back());
    }

    void dispatchEvent(const Event& event) const {
        auto it = m_Listeners.find(event.getType());
        if(it != std::end(m_Listeners)) {
            for(const auto& listener: (*it).second) {
                listener(event);
            }
        }
    }

private:
    ListenerMap m_Listeners;
};

}

#endif // _YGG_EVENTMANAGER_HPP
