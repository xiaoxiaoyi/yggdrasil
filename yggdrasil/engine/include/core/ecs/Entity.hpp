#ifndef __ENTITY_H__
#define __ENTITY_H__

#include <cstdint>

namespace ygg
{

typedef uint32_t EntityID;

class Entity
{
public:
    EntityID getId() const {
        return _id;
    }

    explicit operator bool() const {
        return _id != EntityID(-1);
    }

    bool operator==(const Entity& other) const {
        return _id == other._id;
    }

    bool operator!=(const Entity& other) const {
        return !(*this == other);
    }

    Entity(EntityID id = -1): _id(id) {
    }

    EntityID _id;
};

}

#endif /* __ENTITY_H__ */
