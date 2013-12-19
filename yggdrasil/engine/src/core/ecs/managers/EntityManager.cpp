#include "core/ecs/managers/EntityManager.hpp"
#include <algorithm>

namespace ygg
{
	EntityManager::EntityManager()
		: _count(0), _nextEntityId(0), _totalCreated(0), _totalRemoved(0)
	{

	}

	EntityManager::~EntityManager()
	{

	}

    Entity EntityManager::createEntity(StringID name)
	{
		Entity nextEntity;

		// If an old entity is available for reuse
        if (!_dead.empty())
		{
			// Get last dead entity and bring it back to life
            nextEntity = _dead.back();
            _dead.pop_back();

            _alive.emplace_back(nextEntity);
            _entities[nextEntity.getId()].setName(name);
		}
		else
		{
			EntityID newId = _nextEntityId++;

            // Add entity to master list of entities if this is a completely new entity
            if(newId >= _entities.size()) {
                _entities.resize(newId + 1);
            }
            _entities[newId] = EntityInfo(newId, name);

			// Set entity ID to return
			nextEntity._id = newId;
		}
		
		// Create new entity
        _alive.emplace_back(nextEntity);

        m_NameToEntityMap[name] = nextEntity.getId();

		// Update counters
		_totalCreated++;
		_count++;

		// Return entity id
		return nextEntity;
	}

	void EntityManager::removeEntity(EntityInfo& e)
	{
		// If entity is alive

        auto it = find(begin(_alive), end(_alive), e);
        if (it != end(_alive))
		{
			// Murder entity
            *it = _alive.back();
            _alive.pop_back();
            _dead.emplace_back(e);

            m_NameToEntityMap[e.getName()] = EntityID(-1);
			
			// Reset the component and system bitmasks
			e._componentMask.clear();
			e._systemMask.clear();


			// Update counters
			_count--;
			_totalRemoved++;
		}
	}

	// Get the entity info for an entity
	EntityInfo& EntityManager::getEntityInfo(Entity e) 
	{
		return _entities[e.getId()];
	}
}
