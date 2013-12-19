#include "animation/AnimationSystem.hpp"
#include "core/ecs/World.hpp"

namespace ygg {

const StringID AnimationEvent::CHANGE_FRAME("ChangeFrameEvent");

void AnimationSystem::addAnimationComponent(Entity entity, const Sprite* sprite, StringID defaultAnimationID) {
    _world->addComponent<AnimationComponent>(entity, entity, sprite, defaultAnimationID);
}

void AnimationSystem::dispatchAnimationEvent(const AnimationComponent* pAnimationComponent) {
    _world->dispatchEvent(AnimationEvent(pAnimationComponent->m_Entity, pAnimationComponent->m_CurrentFrameID));
}

void AnimationSystem::onRegistered() {
    registerComponent<AnimationComponent>();
}


void AnimationSystem::process(float dt, Entity e) {
    AnimationComponent* pAnimationComponent = _world->getComponent<AnimationComponent>(e);
    pAnimationComponent->update(dt);

    if(pAnimationComponent->m_bHasChangeFlag){
        dispatchAnimationEvent(pAnimationComponent);
        pAnimationComponent->m_bHasChangeFlag = false;
    }
}

}
