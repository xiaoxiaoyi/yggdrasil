#include "core/ecs/systems/EntitySystem.hpp"
#include <algorithm>

namespace ygg
{
	// Constructor
	EntitySystem::EntitySystem()
        : _world(nullptr), _enabled(true)
	{

	}

	// Indicates that this system has been registered with the world
	void EntitySystem::onRegistered()
	{

	}

	// Called by the client to initiate processing
    void EntitySystem::update(float dt)
	{
		if (checkProcessing())
		{
			begin();
            processEntities(dt, _entities);
			end();
		}
	}

	// Enable processing of this system
	void EntitySystem::setEnabled(bool enabled)
	{
		_enabled = enabled;
	}

	// Get whether this system is enabled
	bool EntitySystem::getEnabled()
	{
		return _enabled;
	}

	// Called during initialisation of this system
	void EntitySystem::initialise()
	{

	}

	// Called before the processing of entities begins
	void EntitySystem::begin()
	{

	}

	// Called after the end of processing
	void EntitySystem::end()
	{

	}

	// Returns whether this system should currently process entities
	bool EntitySystem::checkProcessing()
	{
		return _enabled;
	}

	// Called when an entity is added to this system
	void EntitySystem::added(Entity e)
	{

	}

	// Called when an entity is removed from this system
	void EntitySystem::removed(Entity e)
	{

	}

	// Add an entity
	void EntitySystem::addEntity(EntityInfo& e)
	{
        _entities.emplace_back(e);
	}
		
	// Remove an entity
	void EntitySystem::removeEntity(EntityInfo& e)
    {
        auto it = find(std::begin(_entities), std::end(_entities), e);
        if(it == std::end(_entities)) {
            return;
        }
        *it = _entities.back();
        _entities.pop_back();
	}

	// Set the world
	void EntitySystem::setWorld(World* world)
	{
		_world = world;
        onRegistered();
	}

	// Get the bitmask
	BitMask EntitySystem::getMask()
	{
		return _bitmask;
	}
}
