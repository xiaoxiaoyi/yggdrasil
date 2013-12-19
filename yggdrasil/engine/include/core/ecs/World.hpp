#ifndef __WORLD_H__
#define __WORLD_H__

#include <unordered_map>
#include <tinyxml2.h>
#include <functional>

#include "Entity.hpp"
#include "Component.hpp"

#include "systems/EntityProcessingSystem.hpp"

#include "managers/GroupManager.hpp"
#include "managers/EntityManager.hpp"
#include "managers/SystemManager.hpp"
#include "managers/ManagerManager.hpp"
#include "managers/ComponentManager.hpp"
#include "managers/ComponentTypeManager.hpp"
#include "managers/EventManager.hpp"

#include "exceptions/CompMapUnregistered.hpp"

namespace ygg
{
    typedef std::function<void (Entity e, const tinyxml2::XMLElement* xml)> EntityBuilder;

	// The world that contains all of the entities and systems
	class World
	{
	public:
		// Constructor
        World(EntityBuilder entityBuilder);

		// Destructor
		~World();

		// Add a manager
		template <typename T>
		T* addManager(T& manager);
		
		// Get a manager
		template <typename T>
		T* getManager();

		// Create a new entity
        Entity createEntity(StringID name);

        Entity createEntity(const tinyxml2::XMLElement* xml);

        const EntityInfo& getEntityInfo(Entity e);

        // Add a component to an entity
        template <typename T, typename... Args>
        T* addComponent(Entity e, Args&&... args);

		// Register a system to the system manager
		template <typename T>
		T* addSystem(T& system);

		// Retrieve a system from the system manager
		template <typename T>
		T* getSystem();

		// Get a component from an entity
		template <typename T>
		T* getComponent(Entity e);

        std::vector<Component*> getComponents(Entity e);

		// Remove a component from an entity
		template <typename T>
		void removeComponent(Entity e);
		
		// Remove all components from an entity
		void removeComponents(Entity e);

		// Must be called at the start of each loop to update refreshed
		// and deleted entities
		void loopStart();

		// Update all the systems.
        void update(float dt);

		// Queue an entity for removal
		void remove(Entity e);

		// Refresh an entity
		void refresh(Entity e);

        ListenerID addEventListener(StringID eventType, EventListener listener) {
            return m_EventManager.addEventListener(eventType, listener);
        }

        void removeEventListener(StringID eventType, ListenerID listenerID) {
            m_EventManager.removeEventListener(eventType, listenerID);
        }

        void dispatchEvent(const Event& event) const {
            m_EventManager.dispatchEvent(event);
        }

	protected:
		// Remove entity (called automatically on queued entities)
		void removeEntity(Entity e);

		// Refresh entity (called automatically on queued entities)
		void refreshEntity(Entity e);
		
	private:
		// The manager instances
		GroupManager _groupManager;
		EntityManager _entityManager;
		SystemManager _systemManager;
		ManagerManager _managerManager;
		ComponentManager _componentManager;
        ComponentTypeManager _componentTypeManager;
        EventManager m_EventManager;

		// Entities due to be removed
        std::vector<Entity> _removed;

		// Entities due to be refreshed
        std::vector<Entity> _refreshed;

        EntityBuilder m_BuildEntity;
	};

	// Add a manager
	template <typename T>
	T* World::addManager(T& manager)
	{
		return _managerManager.addManager(manager);
	}

	// Get a manager
	template <typename T>
	T* World::getManager()
	{
		return _managerManager.getManager<T>();
	}
	
	// Add the component to the component manager
    template <typename T, typename... Args>
    T* World::addComponent(Entity e, Args&&... args)
	{
        return _componentManager.addComponent<T>(_entityManager.getEntityInfo(e), std::forward<Args>(args)...);
	}

	// Register a system with the system manager
	template <typename T>
	T* World::addSystem(T& system)
	{
		return _systemManager.addSystem(system);
	}

	// Retrieve a system from the system manager
	template <typename T>
	T* World::getSystem()
	{
		return _systemManager.getSystem<T>();
	}

	// Get a component from an entity
	template <typename T>
	T* World::getComponent(Entity e)
	{
		return _componentManager.getComponent<T>(_entityManager.getEntityInfo(e));
	}

	// Remove a component from an entity
	template <typename T>
	void removeComponent(Entity e);

    // Register a component with this system
    template <typename T>
    void EntitySystem::registerComponent()
    {
        // Add this component
        ComponentTypeManager* componentTypeManager = _world->getManager<ComponentTypeManager>();
        _bitmask.setBit(componentTypeManager->getComponentType<T>());
    }

    // Get a bag for a component type
    template <typename T>
    std::vector<T>* ComponentManager::getComponentBag()
    {
        ComponentContainer<T>* components = (ComponentContainer<T>*)_componentBags[T::getTypeName()];

        // If this type doesn't have a bag yet
        if (components == nullptr)
        {
            // Create one
            components = new ComponentContainer<T>();

            ComponentType componentType = _world->getManager<ComponentTypeManager>()->getComponentType<T>();
            if(componentType >= (int) m_ComponentContainers.size()) {
                m_ComponentContainers.resize(componentType + 1);
            }

            m_ComponentContainers[componentType] = components;

            // Store it in hash map
            _componentBags[T::getTypeName()] = components;
        }

        return &components->m_Container;
    }

    // Add a component to an entity
    template <typename T, typename... Args>
    T* ComponentManager::addComponent(EntityInfo& e, Args&&... args)
    {
        std::vector<T>* components = getComponentBag<T>();
        ComponentType componentType = _world->getManager<ComponentTypeManager>()->getComponentType<T>();

        // Add the component to it
        if(e.getId() >= components->size()) {
            components->resize(e.getId() + 1);
        }
        (*components)[e.getId()] = T(std::forward<Args>(args)...);

        // Set the entity's components bitmask
        e.addComponent(componentType);

        // Return the component we just added
        return getComponent<T>(e);
    }

    // Get a component from an entity
    template <typename T>
    T* ComponentManager::getComponent(EntityInfo& e)
    {
        std::vector<T>* components = getComponentBag<T>();

        // If this entity doesn't have this component return null
        if (!e._componentMask[_world->getManager<ComponentTypeManager>()->getComponentType<T>()])
            return nullptr;

        return &((*components)[e.getId()]);
    }

    inline Component* ComponentManager::getComponent(ComponentType type, EntityInfo& e) {
        IComponentContainer* components = m_ComponentContainers[type];

        // If this entity doesn't have this component return null
        if (!e._componentMask[type])
            return nullptr;

        return components->getComponent(e.getId());
    }
}

#endif /* __WORLD_H__ */
