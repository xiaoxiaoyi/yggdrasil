#include "core/ecs/EntityInfo.hpp"
#include "core/ecs/managers/ComponentManager.hpp"
#include "core/ecs/World.hpp"

namespace ygg
{
	ComponentManager::ComponentManager()
	{
		init();
	}

	ComponentManager::~ComponentManager()
	{
		// Iterate through component bags and deallocate memory
		for (ComponentBagMap::iterator it = _componentBags.begin(); it != _componentBags.end(); ++it)
		{
			// Free bag memory
            delete (*it).second;
		}
	}

	// Initialization function
	void ComponentManager::init()
	{

	}

	// Remove all the components from an entity
	void ComponentManager::removeComponents(EntityInfo& e)
	{
		e._systemMask.clear();
	}

    IComponentContainer* ComponentManager::getComponentBag(const StringID& typeName) {
        IComponentContainer* components = _componentBags[typeName];

        // If this type doesn't have a bag yet
        if (components == nullptr) {
            std::cerr << "No component bag created for " << typeName.toString() << std::endl;
        }

        return components;
    }
}
