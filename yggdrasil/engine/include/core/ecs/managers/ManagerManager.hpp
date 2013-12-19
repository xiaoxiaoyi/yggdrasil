#ifndef GAME_MANAGERMANAGER
#define GAME_MANAGERMANAGER

#include <unordered_map>
#include <cstdlib>

#include "Manager.hpp"
#include "../../StringID.hpp"

namespace ygg
{
	class ManagerManager : public Manager
	{
	public:
        static StringID getTypeName() {
            static StringID name("ManagerManager");
            return name;
        }

		ManagerManager();

		// Get a manager.
		template <typename T>
		T* getManager();
		
		// Add a manager
		template <typename T>
		T* addManager(T& manager);

	private:
		// A hash map of managers by type
        std::unordered_map<StringID, Manager*> _managers;
	};

	// Template functions
	template <typename T>
	T* ManagerManager::getManager() 
	{
        return (T*)_managers[T::getTypeName()];
	}

	template <typename T>
	T* ManagerManager::addManager(T& manager)
	{
		// Initialise manager
		((Manager*)&manager)->initialise(_world);

		// Store a pointer to this manager
        _managers[T::getTypeName()] = &manager;

		// Return a pointer
		return &manager;
    }
}

#endif
