#ifndef _YGG_PLAYERSYSTEM_HPP
#define _YGG_PLAYERSYSTEM_HPP

#include "core/core.hpp"
#include "core/ecs/systems/EntityProcessingSystem.hpp"

namespace ygg {

enum class PlayerAction {
    LEFT,
    RIGHT,
    JUMP,
    ATTACK
};

class PlayerSystem: public EntityProcessingSystem {
public:
    static StringID getTypeName() {
        static StringID name("PlayerSystem");
        return name;
    }
protected:
    virtual void onRegistered();

    virtual void process(float dt, Entity e);

private:
    void doAction(Entity e, PlayerAction action);
};

}

#endif // _YGG_PLAYERSYSTEM_HPP
