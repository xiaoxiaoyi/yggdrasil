#ifndef _YGG_PLAYERCOMPONENT_HPP
#define _YGG_PLAYERCOMPONENT_HPP

#include "core/ecs/Component.hpp"
#include "core/core.hpp"

namespace ygg {

struct PlayerComponent: public Component {
    static StringID getTypeName() {
        static StringID name("PlayerComponent");
        return name;
    }

    HumanInterfaceDevice* m_pHID;

    PlayerComponent(): m_pHID(nullptr) {
    }

    PlayerComponent(HumanInterfaceDevice& hid):
        m_pHID(&hid) {
    }

};

}

#endif // _YGG_PLAYERCOMPONENT_HPP
