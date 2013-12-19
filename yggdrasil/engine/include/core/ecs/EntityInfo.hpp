#ifndef __ENTITYINFO_H__
#define __ENTITYINFO_H__

#include "Entity.hpp"
#include "Component.hpp"
#include "utils/Bitset.hpp"
#include "utils/BitMask.hpp"
#include "../StringID.hpp"

namespace ygg
{
	// The internal representation of an entity
	class EntityInfo
		: public Entity
	{
	public:
		// Constructor
		EntityInfo();

		// Compare the bitmask
		bool compareBitmask(const BitMask& mask) const;

		// Compare the system bitmask
		bool compareSystemBitmask(const BitMask& mask) const;

		// Add a component to this entity
		void addComponent(ComponentType type);

		// Remove a component to this entity
		void removeComponent(ComponentType type);

        StringID getName() const {
            return m_Name;
        }

	protected:
		// We are friends with the entity manager so that it can call
		// this protected constructor
		friend class EntityManager;
		friend class SystemManager;
		friend class ComponentManager;

		// A protected construtor that creates an EntityInfo for a particular
		// entity id
        EntityInfo(EntityID id, StringID name);

        void setName(StringID name) {
            m_Name = name;
        }

		// Which components that are attached to this entity
		BitMask _componentMask;

		// System bitmask
		BitMask _systemMask;

	private:
		// Whether or not this entity is valid
        bool _valid;
        StringID m_Name;
	};
}

#endif /* __ENTITYINFO_H__ */
