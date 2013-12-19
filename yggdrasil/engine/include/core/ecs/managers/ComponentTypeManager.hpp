#ifndef __COMPONENTTYPEMANAGER_H__
#define __COMPONENTTYPEMANAGER_H__

#include <unordered_map>

#include "../Component.hpp"
#include "Manager.hpp"

namespace ygg
{
	// A map of std::type_info hash to ComponentType ID
    typedef std::unordered_map<StringID, ComponentType> ComponentTypeMap;
	
	class ComponentTypeManager
		: public Manager
	{
	public:
        static StringID getTypeName() {
            static StringID name("ComponentTypeManager");
            return name;
        }

		ComponentTypeManager() : _nextId(0) {}

		template <typename T>
        ComponentType getComponentType();

	private:
		ComponentTypeMap _componentTypes;

		ComponentType _nextId;
	};

	template <typename T>
    ComponentType ComponentTypeManager::getComponentType() {
        StringID typeName = T::getTypeName();
        ComponentTypeMap::iterator it = _componentTypes.find(typeName);

        // If hash map does not contain this type
        if (it == _componentTypes.end())
        {
            // Assign it a ComponentType
            ComponentType componentType = _nextId++;

            // Add it to hash map
            _componentTypes[typeName] = componentType;

            // Return it
            return componentType;
        }
        else
        {
            // Return component type
            return (*it).second;
        }
    }
}

#endif /* __COMPONENTTYPEMANAGER_H__ */
