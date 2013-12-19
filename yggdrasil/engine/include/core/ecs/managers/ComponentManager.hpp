#ifndef __COMPONENTMANAGER_H__
#define __COMPONENTMANAGER_H__

#include <unordered_map>
#include <memory>
#include <vector>
#include <tinyxml2.h>

#include "Manager.hpp"
#include "../Component.hpp"
#include "../exceptions/CompMapUnregistered.hpp"
#include "../../StringID.hpp"
#include "../Entity.hpp"

namespace ygg
{
    // An interface for the container of components
    class IComponentContainer {
    public:
        virtual ~IComponentContainer() {
        }

        virtual Component* getComponent(EntityID id) = 0;

        virtual size_t size() const = 0;

        virtual void resize(size_t newSize) = 0;
    };

    // Type erasure
    template<typename T>
    struct ComponentContainer: public IComponentContainer {
        std::vector<T> m_Container;

        virtual Component* getComponent(EntityID id) {
            if(id >= m_Container.size()) {
                return nullptr;
            }
            return &m_Container[id];
        }

        virtual size_t size() const {
            return m_Container.size();
        }

        virtual void resize(size_t newSize) {
            m_Container.resize(newSize);
        }
    };

    typedef std::unordered_map<StringID, IComponentContainer*> ComponentBagMap;

	// The component manager keeps track of which components are attached to which entities
	class ComponentManager
		: public Manager
	{
	public:
        static StringID getTypeName() {
            static StringID name("ComponentManager");
            return name;
        }

		ComponentManager();
		~ComponentManager();

		// Init
        void init();

		// Add a component onto an entity
        template <typename T, typename... Args>
        T* addComponent(EntityInfo& e, Args&&... args);

		// Get a component from an entity
		template <typename T>
		T* getComponent(EntityInfo& e);

        Component* getComponent(ComponentType type, EntityInfo& e);

		// Remove a component from an entity
		template <typename T>
		void removeComponent(EntityInfo& e);

		// Remove all the components from an entity
		void removeComponents(EntityInfo& e);

	protected:
		// Get a bag for a component type
		template <typename T>
        std::vector<T>* getComponentBag();

        IComponentContainer* getComponentBag(const StringID& typeName);

	private:
		// A map of component bags
		ComponentBagMap _componentBags;
        std::vector<IComponentContainer*> m_ComponentContainers;
	};

	// Remove a component from an entity
	template <typename T>
	void ComponentManager::removeComponent(EntityInfo& e)
	{
		e._systemMask.clear(T::type);
	}
}

#endif /* __COMPONENTMAP_H__ */
