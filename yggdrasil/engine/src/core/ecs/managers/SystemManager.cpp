#include "core/ecs/managers/SystemManager.hpp"
#include "core/ecs/Entity.hpp"
#include "core/ecs/EntityInfo.hpp"
#include "core/ecs/systems/EntitySystem.hpp"

namespace ygg
{
	SystemManager::SystemManager()
	{

	}

	// Refresh an entity
	void SystemManager::refresh(EntityInfo& e) 
	{
        for (unsigned int i = 0; i < _systemMap.size(); ++i)
		{
			// Check if this entity should be added.
			if (e.compareBitmask(_systemMap[i]->getMask()))
			{
				// Check if the entity is already in that system.
				if (!e.compareSystemBitmask(BitMask(1) << i))
				{
					_systemMap[i]->addEntity(e);
					e._systemMask.setBit(i);
				}
			}
			else
			{
				// Check if the entity is already in that system.
				if (!e.compareSystemBitmask(BitMask(1) << i))
				{
					_systemMap[i]->removeEntity(e);
					e._systemMask.clearBit(i);
				}
			}
		}
	}

	// Update all the systems.
    void SystemManager::update(float dt)
	{
        for (unsigned int i = 0; i < _systemMap.size(); ++i)
		{
            _systemMap[i]->update(dt);
		}
	}
}
