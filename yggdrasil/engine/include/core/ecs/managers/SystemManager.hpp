#ifndef __SYSTEMMANAGER_H__
#define __SYSTEMMANAGER_H__

#include <unordered_map>
#include <vector>
#include <cstdlib>

#include "Manager.hpp"
#include "../../StringID.hpp"

namespace ygg
{
    class EntitySystem;
    class EntityInfo;

    typedef std::unordered_map<StringID, EntitySystem*> SystemMap;

	// Contains all the systems added to the world
	class SystemManager
		: public Manager
	{
	public:
        static StringID getTypeName() {
            static StringID name("SystemManager");
            return name;
        }

		SystemManager();

		// Register a system with this manager
		template <typename T>
		T* addSystem(T& system);

		// Get system of a particular type
		template <typename T>
		T* getSystem();

		// Refresh an entity with the systems
		void refresh(EntityInfo& e);

		// Update all the systems
        void update(float dt);

	private:
		// An array of systems by ID
		SystemMap _systems;

		// The hash map that contains all the systems
        std::vector<EntitySystem*> _systemMap;
	};

	// Register a system
	template <typename T>
	T* SystemManager::addSystem(T& system)
	{
		// Initialise the system
        system.setWorld(_world);
        _systemMap.emplace_back(&system);
        _systems[T::getTypeName()] = (EntitySystem*)&system;

		return &system;
	}

	// Get system of a particular type
	template <typename T>
	T* SystemManager::getSystem()
	{
        return (T*)_systems[T::getTypeName()];
	}
}

#endif /* __SYSTEMMANAGER_H__ */
