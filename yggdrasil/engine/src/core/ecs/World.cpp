#include "core/ecs/World.hpp"

namespace ygg
{
    World::World(EntityBuilder entityBuilder): m_BuildEntity(entityBuilder)
	{
		// Create manager manager
		_managerManager.initialise(this);
		
		// Initialise default managers
		_managerManager.addManager(_groupManager);
		_managerManager.addManager(_systemManager);
		_managerManager.addManager(_entityManager);
		_managerManager.addManager(_componentManager);
        _managerManager.addManager(_componentTypeManager);
	}

	World::~World()
	{

	}

    Entity World::createEntity(StringID name)
	{
		// Request an unused entity from the entity manager
        return _entityManager.createEntity(name);
	}	

    Entity World::createEntity(const tinyxml2::XMLElement* xml)
    {
        // Request an unused entity from the entity manager
        Entity e = _entityManager.createEntity(StringID(xml->Attribute("id")));
        m_BuildEntity(e, xml);
        return e;
    }

    const EntityInfo& World::getEntityInfo(Entity e) {
        return _entityManager.getEntityInfo(e);
    }

    std::vector<Component*> World::getComponents(Entity e) {
        EntityInfo& info = _entityManager.getEntityInfo(e);
        std::vector<Component*> components;
        for(ComponentType componentType = 0; componentType < COMENT_NUM_BITS; ++componentType) {
            Component* component = _componentManager.getComponent(componentType, info);
            if(component) {
                components.emplace_back(component);
            }
        }
        return components;
    }

	void World::loopStart()
	{

		// Remove entities queued for removal
        for (unsigned int i = 0; i < _removed.size(); ++i)
		{
			removeEntity(_removed[i]);
		}

		_removed.clear();

		// Refresh entities queued for refresh
		// Make sure to do this after updating removed entities, as entities are refreshed after being removed
        for (unsigned int i = 0; i < _refreshed.size(); ++i)
		{
			refreshEntity(_refreshed[i]);
		}

		_refreshed.clear();
	}

    void World::update(float dt)
	{
        _systemManager.update(dt);
	}

	void World::remove(Entity e)
	{
        _removed.emplace_back(e);
	}

	void World::refresh(Entity e) 
	{
        _refreshed.emplace_back(e);
	}

	void World::removeEntity(Entity e)
	{		
		// Get the entity info.
		EntityInfo& info = _entityManager.getEntityInfo(e);

		// Remove the entity.
		_entityManager.removeEntity(info);

		// Refresh systems concerned with this entities
		refresh(e);

		// Remove entity from any groups it's in
		_groupManager.remove(e);
	}

	void World::refreshEntity(Entity e)
	{
		// Get the entity info
		EntityInfo& info = _entityManager.getEntityInfo(e);

		// Refresh the entity
		_systemManager.refresh(info);
	}

}
