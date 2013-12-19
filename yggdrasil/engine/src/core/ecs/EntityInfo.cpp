#include "core/ecs/EntityInfo.hpp"

namespace ygg
{
	// Constructor
	EntityInfo::EntityInfo()
		: _valid(false)
	{
	}

	// Protected Constructor
    EntityInfo::EntityInfo(EntityID id, StringID name)
        : Entity(id), _valid(true), m_Name(name)
	{
	}

	// Compare the bitmask
	bool EntityInfo::compareBitmask(const BitMask& mask) const
	{
		return (mask & _componentMask) == mask;
	}

	// Compare the system bitmask.
	bool EntityInfo::compareSystemBitmask(const BitMask& mask) const
	{
		return (mask & _systemMask) == mask;
	}

	// Add a component
	void EntityInfo::addComponent(ComponentType type)
	{
		_componentMask.setBit(type);
	}

	// Add a component
	void EntityInfo::removeComponent(ComponentType type)
	{
		_componentMask.clearBit(type);
	}
}
