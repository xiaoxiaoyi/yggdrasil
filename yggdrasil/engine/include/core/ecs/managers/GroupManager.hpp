#ifndef __GROUPMANAGER_H__
#define __GROUPMANAGER_H__

#include <string>
#include <unordered_map>
#include <vector>

#include "Manager.hpp"

#include "../Entity.hpp"
#include "../../StringID.hpp"

namespace ygg
{
    typedef std::unordered_map<std::string, std::vector<Entity> > EntityGroupMap;

	class GroupManager
		: public Manager
	{
	public:
        static StringID getTypeName() {
            static StringID name("GroupManager");
            return name;
        }

		GroupManager();
		~GroupManager();

		void set(std::string group, Entity e);

        const std::vector<Entity>& getEntities(std::string group);

		void remove(Entity e);

		std::string getGroup(Entity e);

		bool isGrouped(Entity e);

	private:
		EntityGroupMap _entitiesByGroup;
        std::vector<std::string> _groupsByEntity;

        const std::vector<Entity> _emptyBag;
	};
}

#endif /* __GROUPMANAGER_H__ */
