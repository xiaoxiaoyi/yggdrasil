#ifndef __ENTITYSYSTEM_H__
#define __ENTITYSYSTEM_H__

#include <vector>

#include "../Entity.hpp"
#include "../EntityInfo.hpp"
#include "../utils/BitMask.hpp"
#include "../managers/ComponentTypeManager.hpp"

namespace ygg
{
	// Forward declaration of world
	class World;

	// A system that processes entities
	class EntitySystem
	{
	public:
		// Called by the client to initiate processing
        void update(float dt);

		// Enable processing of this system
		void setEnabled(bool enabled);

		// Get whether this system is enabled
		bool getEnabled();

		// Called during initialisation of this system
		virtual void initialise();

		// Called before the processing of entities begins
		virtual void begin();

		// Called after the end of processing
		virtual void end();

		// Any implementing entity system must implement this method
		// Contains the logic to process the entities of this system
        virtual void processEntities(float dt, std::vector<Entity>& entities) = 0;

		// Returns whether this system should currently process entities
		virtual bool checkProcessing();

		// Called when an entity is added to this system
		virtual void added(Entity e);

		// Called when an entity is removed from this system
		virtual void removed(Entity e);

	protected:
		// Constructor
		EntitySystem();

		// Friends with the system manager so that it can get protected fields
		friend class SystemManager;

		// Indicates that this system has been registered with the world
		virtual void onRegistered();
		
		// Add an entity
		void addEntity(EntityInfo& e);
		
		// Remove an entity
		void removeEntity(EntityInfo& e);

		// Set the world that this system belongs to
		void setWorld(World* world);

		// Register a component with this system
		template <typename T>
		void registerComponent();

		// Get the bitmask created from the combination of components
		// registered with this system
		BitMask getMask();

		// Called when an entity is refreshed
		bool refresh(const EntityInfo& e);

		// The world this system belongs to
		World* _world;

	private:
		// Whether to process this system
		bool _enabled;

		// The bitmask to use for the entity system
		BitMask _bitmask;

    protected:
		// A bag of entities that match this systems bitmask
        std::vector<Entity> _entities;
    };
}

#endif /* __ENTITYSYSTEM_H__ */
