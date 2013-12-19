#ifndef __ENTITYMANAGER_H__
#define __ENTITYMANAGER_H__

#include <unordered_map>
#include <vector>

#include "../Component.hpp"
#include "../EntityInfo.hpp"

#include "Manager.hpp"
#include "core/StringID.hpp"

namespace ygg
{
	class World;

	// A class that manages all of the entities in a world
	class EntityManager
		: public Manager
	{
	public:
        static StringID getTypeName() {
            static StringID name("EntityManager");
            return name;
        }
		// Constructors
		EntityManager();
		~EntityManager();

		// Create an entity (will recycle dead entities ids)
        Entity createEntity(StringID name);

        Entity getEntity(StringID name) const {
            auto it = m_NameToEntityMap.find(name);
            if(it == end(m_NameToEntityMap)) {
                return Entity();
            }
            return Entity((*it).second);
        }

        StringID getName(Entity e) const {
            if(e.getId() >= _entities.size()) {
                return StringID();
            }
            return _entities[e.getId()].getName();
        }

        // Get a reference to the entity info for an entity
        EntityInfo& getEntityInfo(Entity e);

	protected:
		friend class World;
		
		// Remove an entity from the manager
		void removeEntity(EntityInfo& e);

		// Add a component to an entity
		template <typename T>
		void addComponent(Entity e, T c);

	private:
		// Contains all entities
        std::vector<EntityInfo> _entities;

		// Contains the entity IDs of dead or alive entities
        std::vector<Entity> _alive;
        std::vector<Entity> _dead;
		
		// How many entities are in the manager
		int _count;
		EntityID _nextEntityId; // The next valid entity id
		
		// The total amount of removed and added entities
		int _totalCreated;
		int _totalRemoved;

        typedef std::unordered_map<StringID, EntityID> NameToEntityMap;
        NameToEntityMap m_NameToEntityMap;
	};

	// Add a component to an entity
	template <typename T>
	void EntityManager::addComponent(Entity e, T c)
	{
        _entities[e.getId()].addComponent(T::type);
	}
}

#endif /* __ENTITYMANAGER_H__ */
