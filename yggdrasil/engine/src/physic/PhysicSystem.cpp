#include "physic/PhysicSystem.hpp"
#include "core/ecs/World.hpp"

namespace ygg {

const StringID PhysicEvent::STRIKE("StrikeEvent");
const StringID PhysicEvent::REACH_GROUND("ReachGroundEvent");

class DebugDraw: public b2Draw {
    std::vector<glm::vec2> m_Cache;
    std::vector<glm::vec2> m_CircleCache;

    static const size_t CIRCLE_VERTEX_COUNT = 17;
public:
    DebugDraw(): m_Cache(32) {
        float dAngle = glm::pi<float>() * 2.f / (CIRCLE_VERTEX_COUNT - 1);
        float angle = 0.f;
        for(size_t i = 0; i < CIRCLE_VERTEX_COUNT; ++i) {
            angle += dAngle;
            m_CircleCache.push_back(glm::vec2(glm::cos(angle), glm::sin(angle)));
        }

        SetFlags(e_shapeBit | e_centerOfMassBit);
    }

    virtual void DrawPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        if(m_Cache.size() < (size_t) (2 * vertexCount)) {
            m_Cache.resize(vertexCount);
        }
        for(int32 i = 0; i < vertexCount; ++i) {
            m_Cache[i * 2].x = vertices[i].x;
            m_Cache[i * 2].y = vertices[i].y;
            m_Cache[i * 2 + 1].x = vertices[(i + 1) % vertexCount].x;
            m_Cache[i * 2 + 1].y = vertices[(i + 1) % vertexCount].y;
        }

        DebugRenderer::getInstance()->drawLines(m_Cache.data(), 2 * vertexCount, glm::vec3(color.r, color.g, color.b), glm::mat3(1.f));
    }

    virtual void DrawSolidPolygon (const b2Vec2 *vertices, int32 vertexCount, const b2Color &color) {
        DrawPolygon(vertices, vertexCount, color);
    }

    virtual void DrawCircle (const b2Vec2 &center, float32 radius, const b2Color &color) {
        glm::mat3 transform(1.f);
        transform[0][0] = transform[1][1] = radius;
        transform[2][0] = center.x;
        transform[2][1] = center.y;
        DebugRenderer::getInstance()->drawLines(m_CircleCache.data(), m_CircleCache.size(),
                                     glm::vec3(color.r, color.g, color.b), transform);
    }

    virtual void DrawSolidCircle (const b2Vec2 &center, float32 radius, const b2Vec2 &axis, const b2Color &color) {
        DrawCircle(center, radius, color);
    }

    virtual void DrawSegment (const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color) {
        glm::vec2 vertices[] = { glm::vec2(p1.x, p1.y), glm::vec2(p2.x, p2.y) };
        DebugRenderer::getInstance()->drawLines(vertices, 2,
                                     glm::vec3(color.r, color.g, color.b), glm::mat3(1.f));
    }

    virtual void DrawTransform (const b2Transform &xf) {
        glm::vec2 origin(xf.p.x, xf.p.y);
        b2Vec2 xAxis = xf.q.GetXAxis();
        b2Vec2 yAxis = xf.q.GetYAxis();

        glm::vec2 x[] = { origin, origin + glm::vec2(xAxis.x, xAxis.y) };
        DebugRenderer::getInstance()->drawLines(x, 2, glm::vec3(1, 0, 0), glm::mat3(1.f));

        glm::vec2 y[] = { origin, origin + glm::vec2(yAxis.x, yAxis.y) };
        DebugRenderer::getInstance()->drawLines(y, 2, glm::vec3(0, 1, 0), glm::mat3(1.f));
    }
};

static DebugDraw DEBUG_DRAW;

const b2Vec2 PhysicSystem::GRAVITY(0, -5);

PhysicSystem::PhysicSystem():
    m_ContactListener(*this) {
}

void PhysicSystem::init(double dt) {
    m_fDt = dt;
    m_nNextCollisionGroup = 0;
    m_nStaticGroup = addCollisionGroup();

    m_pWorld.reset(new b2World(GRAVITY));
    m_pWorld->SetDebugDraw(&DEBUG_DRAW);
    m_pWorld->SetContactListener(&m_ContactListener);
    m_pWorld->SetContactFilter(&m_ContactFilter);
}

void PhysicSystem::clear() {
    init(m_fDt);
}

void PhysicSystem::addStaticChain(const glm::vec2* first, size_t count, bool loop) {
    std::vector<b2Vec2> vertices;

    for(const glm::vec2* it = first; it != first + count; ++it) {
        vertices.push_back(b2Vec2(it->x, it->y));
    }

    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(0, 0);
    b2Body* body = m_pWorld->CreateBody(&bodyDef);

    b2ChainShape chain;
    if(loop) {
        chain.CreateLoop(vertices.data(), vertices.size());
    } else {
        chain.CreateChain(vertices.data(), vertices.size());
    }

    b2FixtureDef fixture;
    fixture.shape = &chain;
    fixture.restitution = 0.f;
    fixture.friction = 50.f;
    fixture.filter.groupIndex = -(int32_t) m_nStaticGroup;
    fixture.filter.categoryBits = STATIC_GEOMETRY_CATEGORY_BITS;
    fixture.filter.maskBits = BODY_CATEGORY_BITS;

    body->CreateFixture(&fixture);
}

PhysicComponent* PhysicSystem::addDynamicBody(Entity e, const glm::vec2& position,
                                              const Sprite* pSprite, uint32_t collisionGroup) {
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = b2Vec2(position.x, position.y);
    bodyDef.fixedRotation = true;
    b2Body* pBody = m_pWorld->CreateBody(&bodyDef);
    return _world->addComponent<PhysicComponent>(e, e, pBody, pSprite, collisionGroup);
}

uint32_t PhysicSystem::addCollisionGroup() {
    return m_nNextCollisionGroup++;
}

void PhysicSystem::update(float dt) {
    const uint32_t velocityIterations = 6;
    const uint32_t positionIterations = 3;

    float dtCopy = dt;
    while(dtCopy > 0.f) {
        float step = m_fDt;
        if(dtCopy - m_fDt < 0.f) {
            step = dtCopy;
        }
        m_pWorld->Step(step, velocityIterations, positionIterations);
        dtCopy -= m_fDt;
    }

    Super::update(dt);
}

void PhysicSystem::process(float dt, Entity e) {
    PositionComponent* position = _world->getComponent<PositionComponent>(e);
    PhysicComponent* physic = _world->getComponent<PhysicComponent>(e);
    position->m_Position = physic->getPosition();
}

void PhysicSystem::renderDebug() const {
    m_pWorld->DrawDebugData();
}

void PhysicSystem::dispatchEvent(const Event& event) {
    _world->dispatchEvent(event);
}

PhysicSystem::ContactListener::ContactListener(PhysicSystem& physicSystem):
    m_PhysicSystem(physicSystem) {
}

union PtrUnion {
    void* ptr;
    unsigned int value;
};

static Entity getEntity(const b2Fixture* pFixture) {
    PtrUnion u;
    u.ptr = pFixture->GetUserData();
    return Entity(u.value);
}

void PhysicSystem::ContactListener::BeginContact(b2Contact* contact) {
    if(contact->GetFixtureA()->GetFilterData().categoryBits == BODY_CATEGORY_BITS
            && contact->GetFixtureB()->GetFilterData().categoryBits == STATIC_GEOMETRY_CATEGORY_BITS) {
        Entity entity = getEntity(contact->GetFixtureA());

        PhysicComponent* pPhysicComponent = m_PhysicSystem._world->getComponent<PhysicComponent>(entity);

        if(!pPhysicComponent->m_bIsOnGround) {
            pPhysicComponent->m_bIsOnGround = true;
            m_PhysicSystem.dispatchEvent(ReachGroundEvent(entity));
        }
    }
    if(contact->GetFixtureB()->GetFilterData().categoryBits == BODY_CATEGORY_BITS
            && contact->GetFixtureA()->GetFilterData().categoryBits == STATIC_GEOMETRY_CATEGORY_BITS) {
        Entity entity = getEntity(contact->GetFixtureB());

        PhysicComponent* pPhysicComponent = m_PhysicSystem._world->getComponent<PhysicComponent>(entity);

        if(!pPhysicComponent->m_bIsOnGround) {
            pPhysicComponent->m_bIsOnGround = true;
            m_PhysicSystem.dispatchEvent(ReachGroundEvent(entity));
        }
    }
    if(contact->GetFixtureA()->GetFilterData().categoryBits == STRIKE_CATEGORY_BITS) {
        Entity entity1 = getEntity(contact->GetFixtureA());
        Entity entity2 = getEntity(contact->GetFixtureB());

        m_PhysicSystem.dispatchEvent(StrikeEvent(entity1, entity2));
    }
    if(contact->GetFixtureB()->GetFilterData().categoryBits == STRIKE_CATEGORY_BITS) {
        Entity entity1 = getEntity(contact->GetFixtureA());
        Entity entity2 = getEntity(contact->GetFixtureB());

        m_PhysicSystem.dispatchEvent(StrikeEvent(entity2, entity1));
    }
}

void PhysicSystem::ContactListener::EndContact(b2Contact* contact) {

}

void PhysicSystem::ContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold) {

}

void PhysicSystem::ContactListener::PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) {
}

}
