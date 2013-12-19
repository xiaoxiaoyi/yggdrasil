#include "physic/PhysicComponent.hpp"
#include "physic/PhysicSystem.hpp"

namespace ygg {

static void copy(const glm::vec2 vertices[4], b2Vec2* b2Vertices) {
    for(int i = 0; i < 4; ++i) {
        b2Vertices[i] = b2Vec2(vertices[i].x, vertices[i].y);
    }
}

PhysicComponent::PhysicComponent(Entity entity, b2Body* pBody, const Sprite* sprite,
                                 uint32_t collisionGroup):
    m_Entity(entity), m_pBody(pBody), m_pSprite(sprite), m_nCollisionGroup(collisionGroup),
    m_bIsOnGround(false) {
    setFrame(0);
}

void PhysicComponent::setFrame(SpriteFrameID frameID) {
    const SpriteBoxArray& boxSet = m_pSprite->getBoxes(frameID);

    b2Fixture* ptr = m_pBody->GetFixtureList();
    while(ptr) {
        b2Fixture* next = ptr->GetNext();
        m_pBody->DestroyFixture(ptr);
        ptr = next;
    }

    for(const auto& box: boxSet) {
        b2PolygonShape shape;
        b2Vec2 vertices[4];
        copy(box.m_Vertices, vertices);
        shape.Set(vertices, 4);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.userData = reinterpret_cast<void*>(m_Entity._id);
        fixture.filter.groupIndex = -(int32_t) m_nCollisionGroup;

        if(box.m_Type == SpriteBox::BODY) {
            fixture.density = 0.1f;
            fixture.friction = 0.2f;
            fixture.restitution = 0.f;
            fixture.isSensor = false;
            fixture.filter.categoryBits = PhysicSystem::BODY_CATEGORY_BITS;
            fixture.filter.maskBits = PhysicSystem::STATIC_GEOMETRY_CATEGORY_BITS | PhysicSystem::BODY_CATEGORY_BITS;
        } else {
            fixture.density = 0.f;
            fixture.friction = 0.f;
            fixture.restitution = 0.f;
            fixture.isSensor = true;
            if(box.m_Type == SpriteBox::HURT) {
                fixture.filter.categoryBits = PhysicSystem::HURT_CATEGORY_BITS;
                fixture.filter.maskBits = PhysicSystem::STRIKE_CATEGORY_BITS;
            } else if(box.m_Type == SpriteBox::STRIKE) {
                fixture.filter.categoryBits = PhysicSystem::STRIKE_CATEGORY_BITS;
                fixture.filter.maskBits = PhysicSystem::HURT_CATEGORY_BITS;
            }
        }

        m_pBody->CreateFixture(&fixture);
    }
    b2MassData mass;
    mass.mass = 0.01f;
    mass.center = b2Vec2(0, 0);
    mass.I = 0.f;
    m_pBody->SetMassData(&mass);
}

void PhysicComponent::moveLeft() {
    b2Vec2 velocity = m_pBody->GetLinearVelocity();
    m_pBody->SetLinearVelocity(b2Vec2(-1, velocity.y));
}

void PhysicComponent::moveRight() {
    b2Vec2 velocity = m_pBody->GetLinearVelocity();
    m_pBody->SetLinearVelocity(b2Vec2(1, velocity.y));
}

void PhysicComponent::jump() {
    if(!m_bIsOnGround) {
        return;
    }

    b2Vec2 velocity = m_pBody->GetLinearVelocity();
    m_pBody->ApplyLinearImpulse(b2Vec2(0., 0.05 * exp(-velocity.y * velocity.y)), m_pBody->GetWorldCenter());
    m_bIsOnGround = false;
}

}
