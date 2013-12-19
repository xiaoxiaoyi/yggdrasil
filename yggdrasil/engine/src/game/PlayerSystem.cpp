#include "game/PlayerSystem.hpp"
#include "game/PlayerComponent.hpp"
#include "physic/PhysicComponent.hpp"
#include "animation/AnimationComponent.hpp"

namespace ygg {

void PlayerSystem::onRegistered() {
    registerComponent<PlayerComponent>();
    registerComponent<PhysicComponent>();
    registerComponent<AnimationComponent>();
}

void PlayerSystem::doAction(Entity entity, PlayerAction action) {
    switch(action) {
    case PlayerAction::LEFT: {
        PhysicComponent* physicComponent = _world->getComponent<PhysicComponent>(entity);
        physicComponent->moveLeft();
        break;
    }
    case PlayerAction::RIGHT: {
        PhysicComponent* physicComponent = _world->getComponent<PhysicComponent>(entity);
        physicComponent->moveRight();
        break;
    }
    case PlayerAction::JUMP: {
        AnimationComponent* animationComponent = _world->getComponent<AnimationComponent>(entity);
        PhysicComponent* physicComponent = _world->getComponent<PhysicComponent>(entity);

        physicComponent->jump();
        animationComponent->setAnimation(StringID("jump"), AnimationPlayState::LOOP);

        break;
    }
    case PlayerAction::ATTACK: {
        AnimationComponent* animationComponent = _world->getComponent<AnimationComponent>(entity);
        animationComponent->setAnimation(StringID("attack"), AnimationPlayState::ONCE);
        break;
    }
    }
}

void PlayerSystem::process(float dt, Entity e) {
    PlayerComponent* pPlayerComponent = _world->getComponent<PlayerComponent>(e);
    HumanInterfaceDevice* pHID = pPlayerComponent->m_pHID;
    assert(pHID);
    pHID->update();

    if(pHID->isPressed(HumanInterfaceDevice::LEFT)) {
        doAction(e, PlayerAction::LEFT);
    }

    if(pHID->isPressed(HumanInterfaceDevice::RIGHT)) {
        doAction(e, PlayerAction::RIGHT);
    }

    if(pHID->detectPressedEvent(HumanInterfaceDevice::UP)) {
        doAction(e, PlayerAction::JUMP);
    }

    if(pHID->detectPressedEvent(HumanInterfaceDevice::BUTTON0)) {
        doAction(e, PlayerAction::ATTACK);
    }
}

}
