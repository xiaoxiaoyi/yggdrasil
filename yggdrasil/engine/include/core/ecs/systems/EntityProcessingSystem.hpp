#ifndef __ENTITYPROCESSINGSYSTEM_H__
#define __ENTITYPROCESSINGSYSTEM_H__

#include <vector>

#include "../Entity.hpp"
#include "../systems/EntitySystem.hpp"

namespace ygg
{
	class EntityProcessingSystem
        : public EntitySystem
    {
	public:
		EntityProcessingSystem();

		// Process a single entity, override to specify behaviour
        virtual void process(float dt, Entity e) = 0;

		// Whether to currently process this entity
		virtual bool checkProcessing();

		// Automatically call process for each entity
        void processEntities(float dt, std::vector<Entity>& entities);
	};
};

#endif /* __ENTITYPROCESSINGSYSTEM_H__ */
