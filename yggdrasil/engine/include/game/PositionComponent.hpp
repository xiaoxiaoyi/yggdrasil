#ifndef _YGG_POSITIONCOMPONENT_HPP
#define _YGG_POSITIONCOMPONENT_HPP

#include "core/ecs/Component.hpp"
#include "core/core.hpp"

namespace ygg {

struct PositionComponent: public Component {
    static StringID getTypeName() {
        static StringID name("PositionComponent");
        return name;
    }

    virtual void display(TwBar *bar) {
        TwAddButton(bar, "PositionComponent", nullptr, nullptr, "");
        TwAddVarRO(bar, "PositionComponent::x", TW_TYPE_FLOAT, &m_Position.x, " label=x ");
        TwAddVarRO(bar, "PositionComponent::y", TW_TYPE_FLOAT, &m_Position.y, " label=y ");
    }

    PositionComponent() {
    }

    PositionComponent(glm::vec2 position):
        m_Position(position) {
    }

    glm::vec2 m_Position;
};

}

#endif // _YGG_POSITIONCOMPONENT_HPP
