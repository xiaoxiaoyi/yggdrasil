#ifndef _YGG_PHYSICSYSTEM_HPP
#define _YGG_PHYSICSYSTEM_HPP

#include "core/core.hpp"
#include "core/ecs/systems/EntityProcessingSystem.hpp"

#include "PhysicComponent.hpp"

#include "game/PositionComponent.hpp"

#include <Box2D/Box2D.h>

namespace ygg {

struct PhysicEvent: public Event {
    static const StringID STRIKE;
    static const StringID REACH_GROUND;
};

struct StrikeEvent: public PhysicEvent {
public:
    virtual StringID getType() const {
        return PhysicEvent::STRIKE;
    }

    Entity m_SourceEntity;
    Entity m_TargetEntity;

    StrikeEvent(Entity srcEntity, Entity targetEntity):
        m_SourceEntity(srcEntity), m_TargetEntity(targetEntity) {
    }
};

struct ReachGroundEvent: public PhysicEvent {
public:
    virtual StringID getType() const {
        return PhysicEvent::REACH_GROUND;
    }

    Entity m_Entity;

    ReachGroundEvent(Entity entity):
        m_Entity(entity) {
    }
};

class PhysicSystem: public EntityProcessingSystem {
    typedef EntityProcessingSystem Super;

    static const b2Vec2 GRAVITY;

protected:
    void onRegistered() {
        registerComponent<PhysicComponent>();
        registerComponent<PositionComponent>();
    }

public:
    static const uint16_t STATIC_GEOMETRY_CATEGORY_BITS = 1 << 0;
    static const uint16_t BODY_CATEGORY_BITS = 1 << 1;
    static const uint16_t HURT_CATEGORY_BITS = 1 << 2;
    static const uint16_t STRIKE_CATEGORY_BITS = 1 << 3;

    static StringID getTypeName() {
        static StringID name("PhysicSystem");
        return name;
    }

    PhysicSystem();

    void init(double dt);

    void clear();

    void addStaticChain(const glm::vec2* first, size_t count,
                        bool loop);

    PhysicComponent* addDynamicBody(Entity e, const glm::vec2& position,
                                    const Sprite* pSprite, uint32_t collisionGroup);

    virtual void update(float dt);

    void renderDebug() const;

    uint32_t addCollisionGroup();

    void process(float dt, Entity e);

private:
    float m_fDt;
    std::unique_ptr<b2World> m_pWorld;

    struct ContactListener: public b2ContactListener {
        PhysicSystem& m_PhysicSystem;
    public:
        ContactListener(PhysicSystem& physicSystem);

        virtual void BeginContact(b2Contact* contact);

        virtual void EndContact(b2Contact* contact);

        virtual void PreSolve(b2Contact* contact,
                              const b2Manifold* oldManifold);

        virtual void PostSolve(b2Contact* contact,
                               const b2ContactImpulse* impulse);
    };

    ContactListener m_ContactListener;
    b2ContactFilter m_ContactFilter;

    uint32_t m_nNextCollisionGroup;
    uint32_t m_nStaticGroup;

    void dispatchEvent(const Event& event);
};

}

#endif // _YGG_PHYSICSYSTEM_HPP
