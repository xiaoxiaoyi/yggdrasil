#ifndef _YGG_GAMEESYSTEM_HPP_
#define _YGG_GAMEESYSTEM_HPP_

#include "core/core.hpp"
#include "core/ecs/World.hpp"

#include "render/RenderSystem.hpp"
#include "physic/PhysicSystem.hpp"
#include "animation/AnimationSystem.hpp"
#include "PlayerSystem.hpp"

#include "Map.hpp"

#include <SFML/Window.hpp>
#include <Box2D/Box2D.h>

namespace ygg {

class GameSystem {
public:
    friend class Player;

    GameSystem(SystemInterface& systemInterface);

    void setMap(const std::string& map);

    void handleEvent(const sf::Event& e);

    /**
     * @brief Run an iteration of the game.
     * @param elapsedTime The time elapsed in seconds.
     */
    void runGameIteration(double elapsedTime);

private:
    void processEvent(const ReachGroundEvent& e);

    void processEvent(const StrikeEvent& e);

    void processEvent(const AnimationEvent& e);

    // This function is a callback called by the world to initialize entity with xml data
    void buildEntity(Entity e, const tinyxml2::XMLElement* xml);

    SystemInterface& m_SystemInterface;
    AssetLibrary m_AssetLibrary;

    // The world managing all the entities
    World m_World;

    Entity m_PlayerEntity; // Entity corresponding to the player

    // All the systems
    RenderSystem m_RenderSystem;
    PhysicSystem m_PhysicSystem;
    AnimationSystem m_AnimationSystem;
    PlayerSystem m_PlayerSystem;

    DebugRenderer m_DebugRenderer;

    uint32_t m_PlayerGroup, m_EnemyGroup;
};

}

#endif
