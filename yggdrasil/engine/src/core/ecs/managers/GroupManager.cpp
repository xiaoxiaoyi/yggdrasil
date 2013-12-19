#include "core/ecs/managers/GroupManager.hpp"

namespace ygg
{
	GroupManager::GroupManager()
	{

	}

	GroupManager::~GroupManager()
	{

	}

	void GroupManager::set(std::string group, Entity e)
	{
		// Make sure entity only belongs to one group
		remove(e);

		// Find group in map
		EntityGroupMap::iterator it = _entitiesByGroup.find(group);

		if (it != _entitiesByGroup.end())
		{
			// Add to group
            it->second.emplace_back(e);
		}
		else
		{
            std::vector<Entity> bag;

			// Insert fresh bag into map for this group
			std::pair<EntityGroupMap::iterator, bool> result = _entitiesByGroup.insert(EntityGroupMap::value_type(group, bag));

			// Add it to the new group bag
            result.first->second.emplace_back(e);
		}
		
		// Set entity's group
        _groupsByEntity[e.getId()] = group;
	}

    const std::vector<Entity>& GroupManager::getEntities(std::string group)
	{
		// Find group
		EntityGroupMap::iterator it = _entitiesByGroup.find(group);

		// If this group exists
		if (it != _entitiesByGroup.end())
		{
			// Get reference to entities
            std::vector<Entity>& entities = _entitiesByGroup.find(group)->second;

			return entities;
		}

		return _emptyBag;
	}

	void GroupManager::remove(Entity e)
	{
		// If the entity can possibly by in a group
        if (e.getId() < _groupsByEntity.size())
		{
			std::string group = _groupsByEntity[e.getId()];

			// If this group exists
			if (group != "")
			{
				// Unset group
                _groupsByEntity[e.getId()] = "";

				// Get group bag
				EntityGroupMap::iterator it =  _entitiesByGroup.find(group);
                std::vector<Entity>& group = it->second;

                group[e.getId()] = group.back();
                group.pop_back();
			}
		}
	}

	std::string GroupManager::getGroup(Entity e)
	{
        if (e.getId() < _groupsByEntity.size())
			return _groupsByEntity[e.getId()];
		else
			return "";
	}

	bool GroupManager::isGrouped(Entity e)
	{
		// Return false if this isn't in a group
		if (getGroup(e) == "")
			return false;
		else
			return true;
	}
}
