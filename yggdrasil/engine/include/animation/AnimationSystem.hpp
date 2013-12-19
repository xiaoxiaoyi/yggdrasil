#ifndef _YGG_ANIMATIONSYSTEM_HPP_
#define _YGG_ANIMATIONSYSTEM_HPP_

#include "core/core.hpp"
#include "core/ecs/systems/EntityProcessingSystem.hpp"

#include "AnimationComponent.hpp"

namespace ygg {

/**
 * @brief The AnimationEvent struct notify that an animation component has changed
 * this frame.
 */
struct AnimationEvent: public Event {
    static const StringID CHANGE_FRAME;

    virtual StringID getType() const {
        return CHANGE_FRAME;
    }

    //! The game object associated with the animation component
    Entity m_Entity;
    //! The new frame ID of the component
    SpriteFrameID m_FrameID;

    AnimationEvent(Entity entity, SpriteFrameID frameID):
        m_Entity(entity), m_FrameID(frameID) {
    }
};

class AnimationSystem: public EntityProcessingSystem {
protected:
    virtual void onRegistered();

public:
    static StringID getTypeName() {
        static StringID name("AnimationSystem");
        return name;
    }

    void addAnimationComponent(Entity entity, const Sprite* sprite, StringID defaultAnimationID);

    virtual void process(float dt, Entity e);

private:
    void dispatchAnimationEvent(const AnimationComponent* pAnimationComponent);
};

}

#endif
