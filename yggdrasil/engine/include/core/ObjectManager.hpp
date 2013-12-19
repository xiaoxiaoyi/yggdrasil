#ifndef _YGG_OBJECTMANAGER_HPP
#define _YGG_OBJECTMANAGER_HPP

#include <cstdint>
#include <vector>
#include <memory>

namespace ygg {

struct ObjectID {
public:
    union {
        struct {
            uint16_t uniqueID; //! The uniqueID of the object is unique for a particular manager
            uint16_t index; //! The index is the cell of the array containing the object
        };
        uint32_t completeID;
    };

    ObjectID(uint16_t uniqueID = 0, uint16_t index = 0):
        uniqueID(uniqueID), index(index) {
    }

    explicit ObjectID(uint32_t completeID):
        completeID(completeID) {
    }

    explicit operator bool() const {
        return uniqueID != 0;
    }
};

struct NoFragmentationPolicy {

};

struct NoRelocationPolicy {
};

/**
 * The class ObjectManager allow for efficient memory management of a collection of objects
 */
template<typename ObjectType, typename AllocationPolicy = NoFragmentationPolicy>
class ObjectManager {
    struct Data {
        uint16_t m_nIndex;
        ObjectType m_Object;

        template<typename... Args>
        Data(uint16_t index, Args&&... args):
            m_nIndex(index), m_Object(std::forward<Args>(args)...){
        }
    };

    template<typename InnerIterator, typename RefType>
    class IteratorTemplate {
    public:
        typedef ObjectType reference;

        IteratorTemplate(InnerIterator it): _it(it) {}

        RefType operator*() const { return (*_it).m_Object; }

        IteratorTemplate& operator++() { ++_it; return *this; }
        IteratorTemplate operator++(int) { iterator tmp(*this); ++*this; return tmp; }

        bool operator==(IteratorTemplate const& it) const { return _it == it._it; }
        bool operator!=(IteratorTemplate const& it) const { return !(*this == it); }

    private:
        InnerIterator _it;
    };

public:
    typedef std::vector<ObjectID> IDContainer;
    typedef std::vector<Data> DataContainer;

    typedef IteratorTemplate<typename DataContainer::iterator, ObjectType&> iterator;
    typedef IteratorTemplate<typename DataContainer::const_iterator, const ObjectType&> const_iterator;

    ObjectManager(): m_nNextUniqueID(1), m_nFirstFree(0) {
    }

    template<typename... Args>
    ObjectID add(Args&&... args) {
        // If no free index, create a new one
        if(!m_nFirstFree) {
            ObjectID id(m_nNextUniqueID++, m_Indices.size()); // Indirect id

            m_Indices.emplace_back(); // Direct id
            m_Indices.back().uniqueID = id.uniqueID;
            m_Indices.back().index = m_Data.size();
            m_Data.emplace_back(id.index, std::forward<Args>(args)...);

            return id;
        }

        // Else grap the first free index and fill it
        uint16_t firstFree = m_nFirstFree - 1; // True index of the first free
        m_nFirstFree = m_Indices[firstFree].index; // New first free

        ObjectID id(m_nNextUniqueID++, firstFree); // Indirect id

        m_Indices[firstFree].uniqueID = id.uniqueID;
        m_Indices[firstFree].index = m_Data.size();
        m_Data.emplace_back(id.index, std::forward<Args>(args)...);

        return id;
    }

    void remove(ObjectID id) {
        if(!isValid(id)) {
            return;
        }

        // The direct id of the data to remove
        ObjectID& directID = m_Indices[id.index];

        // Exchange the last data and the data to remove
        m_Indices[m_Data.back().m_nIndex].index = directID.index;
        std::swap(m_Data[directID.index], m_Data.back());
        m_Data.pop_back();

        // Change the first free index
        directID.index = m_nFirstFree;
        directID.uniqueID = 0;
        m_nFirstFree = id.index + 1;
    }

    bool isValid(ObjectID id) const {
        // The object is valid if it has'nt been replaced by another object
        return m_Indices[id.index].uniqueID == id.uniqueID;
    }

    const ObjectType& operator [](ObjectID id) const {
        assert(id.index < m_Indices.size());
        assert(isValid(id));
        return m_Data[m_Indices[id.index].index].m_Object;
    }

    ObjectType& operator [](ObjectID id) {
        assert(id.index < m_Indices.size());
        assert(isValid(id));
        return m_Data[m_Indices[id.index].index].m_Object;
    }

    const_iterator begin() const {
        return std::begin(m_Data);
    }

    const_iterator end() const {
        return std::end(m_Data);
    }

    iterator begin() {
        return std::begin(m_Data);
    }

    iterator end() {
        return std::end(m_Data);
    }

    void clear() {
        m_Indices.clear();
        m_Data.clear();
        m_nFirstFree = 0;
        m_nNextUniqueID = 1;
    }

private:
    uint16_t m_nNextUniqueID;
    uint16_t m_nFirstFree;
    IDContainer m_Indices;
    DataContainer m_Data;
};

template<typename ObjectType>
class ObjectManager<ObjectType, NoRelocationPolicy> {
public:
    struct Data {
        uint16_t m_nIndex;
        std::unique_ptr<ObjectType> m_pObject;

        template<typename... Args>
        Data(uint16_t index, Args&&... args):
            m_nIndex(index), m_pObject(new ObjectType(std::forward<Args>(args)...)) {
        }
    };

    template<typename InnerIterator, typename RefType>
    class IteratorTemplate {
    public:
        typedef ObjectType reference;

        IteratorTemplate(InnerIterator it): _it(it) {}

        RefType operator*() const { return *(*_it).m_pObject; }

        IteratorTemplate& operator++() { ++_it; return *this; }
        IteratorTemplate operator++(int) { iterator tmp(*this); ++*this; return tmp; }

        bool operator==(IteratorTemplate const& it) const { return _it == it._it; }
        bool operator!=(IteratorTemplate const& it) const { return !(*this == it); }

    private:
        InnerIterator _it;
    };

public:
    typedef std::vector<ObjectID> IDContainer;
    typedef std::vector<Data> DataContainer;

    typedef IteratorTemplate<typename DataContainer::iterator, ObjectType&> iterator;
    typedef IteratorTemplate<typename DataContainer::const_iterator, const ObjectType&> const_iterator;

    ObjectManager(): m_nNextUniqueID(1), m_nFirstFree(0) {
    }

    template<typename... Args>
    ObjectID add(Args&&... args) {
        // If no free index, create a new one
        if(!m_nFirstFree) {
            ObjectID id(m_nNextUniqueID++, m_Indices.size()); // Indirect id

            m_Indices.emplace_back(); // Direct id
            m_Indices.back().uniqueID = id.uniqueID;
            m_Indices.back().index = m_Data.size();
            m_Data.emplace_back(id.index, std::forward<Args>(args)...);

            return id;
        }

        // Else grap the first free index and fill it
        uint16_t firstFree = m_nFirstFree - 1; // True index of the first free
        m_nFirstFree = m_Indices[firstFree].index; // New first free

        ObjectID id(m_nNextUniqueID++, firstFree); // Indirect id

        m_Indices[firstFree].uniqueID = id.uniqueID;
        m_Indices[firstFree].index = m_Data.size();
        m_Data.emplace_back(id.index, std::forward<Args>(args)...);

        return id;
    }

    void remove(ObjectID id) {
        if(!isValid(id)) {
            return;
        }

        // The direct id of the data to remove
        ObjectID& directID = m_Indices[id.index];

        // Exchange the last data and the data to remove
        m_Indices[m_Data.back().m_nIndex].index = directID.index;
        std::swap(m_Data[directID.index], m_Data.back());
        m_Data.pop_back();

        // Change the first free index
        directID.index = m_nFirstFree;
        directID.uniqueID = 0;
        m_nFirstFree = id.index + 1;
    }

    bool isValid(ObjectID id) const {
        // The object is valid if it has'nt been replaced by another object
        return m_Indices[id.index].uniqueID == id.uniqueID;
    }

    const ObjectType& operator [](ObjectID id) const {
        assert(id.index < m_Indices.size());
        assert(isValid(id));
        return *m_Data[m_Indices[id.index].index].m_pObject;
    }

    ObjectType& operator [](ObjectID id) {
        assert(id.index < m_Indices.size());
        assert(isValid(id));
        return *m_Data[m_Indices[id.index].index].m_pObject;
    }

    const_iterator begin() const {
        return std::begin(m_Data);
    }

    const_iterator end() const {
        return std::end(m_Data);
    }

    iterator begin() {
        return std::begin(m_Data);
    }

    iterator end() {
        return std::end(m_Data);
    }

    void clear() {
        m_Indices.clear();
        m_Data.clear();
        m_nFirstFree = 0;
        m_nNextUniqueID = 1;
    }

private:
    uint16_t m_nNextUniqueID;
    uint16_t m_nFirstFree;
    IDContainer m_Indices;
    DataContainer m_Data;
};

}

#endif // _YGG_OBJECTMANAGER_HPP
