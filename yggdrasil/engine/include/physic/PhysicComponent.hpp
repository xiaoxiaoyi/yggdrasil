#ifndef _YGG_PHYSICCOMPONENT_HPP
#define _YGG_PHYSICCOMPONENT_HPP

#include "core/core.hpp"
#include "core/ecs/Component.hpp"

#include <Box2D/Box2D.h>

namespace ygg {

class PhysicSystem;

struct PhysicComponent: public Component {
    static StringID getTypeName() {
        static StringID name("PhysicComponent");
        return name;
    }

    PhysicComponent() {
    }

    PhysicComponent(Entity entity, b2Body* pBody, const Sprite* sprite,
                    uint32_t collisionGroup);

    void setFrame(SpriteFrameID frameID);

    void moveLeft();

    void moveRight();

    void jump();

    glm::vec2 getPosition() const {
        b2Vec2 position = m_pBody->GetPosition();
        return glm::vec2(position.x, position.y);
    }

    virtual void display(TwBar* bar) {
        TwAddButton(bar, "PhysicComponent", nullptr, nullptr, "");
        TwAddVarRO(bar, "PhysicComponent::x", TW_TYPE_FLOAT, &m_pBody->GetPosition().x, " label=x ");
        TwAddVarRO(bar, "PhysicComponent::y", TW_TYPE_FLOAT, &m_pBody->GetPosition().y, " label=y ");
        TwAddVarRO(bar, "PhysicComponent::collisionGroup", TW_TYPE_UINT32, &m_nCollisionGroup, " label=collisionGroup ");
        TwAddVarRO(bar, "PhysicComponent::isOnGround", TW_TYPE_BOOLCPP, &m_bIsOnGround, " label=isOnGround ");
    }

    Entity m_Entity;
    b2Body* m_pBody;
    const Sprite* m_pSprite;
    uint32_t m_nCollisionGroup;
    bool m_bIsOnGround;
};

}

#endif // _YGG_PHYSICCOMPONENT_HPP
