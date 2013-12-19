#ifndef _YGG_STRINGID_HPP
#define _YGG_STRINGID_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <iostream>

namespace ygg {

uint32_t crc32(const char* str);

class StringID {
    typedef std::unordered_map<uint32_t, std::string> IDMap;

    static IDMap& getGlobalIDMap() {
        static IDMap map;
        return map;
    }

    static bool registerID(const std::string& str, uint32_t hashCode) {
        IDMap& map = getGlobalIDMap();

        auto it = map.find(hashCode);
        if(it != end(map) && str != (*it).second) {
            std::cerr << "Collision detected for the id " << str << " versus the id " << (*it).second << "." << std::endl;
            return false;
        }
        map[hashCode] = str;
        return true;
    }

    /*
    static bool registerID(const std::string& str) {
        return registerID(str, crc32(str.c_str()));
    }*/

public:
    static StringID UNDEFINED;

    StringID(): m_nHashCode(UNDEFINED.m_nHashCode) {
    }

    explicit StringID(const char* str):
        m_nHashCode(crc32(str)) {
        registerID(str, m_nHashCode);
    }

    explicit StringID(const std::string& str):
        m_nHashCode(crc32(str.c_str())) {
        registerID(str, m_nHashCode);
    }

    uint32_t getHashCode() const {
        return m_nHashCode;
    }

    std::string toString() const {
        IDMap& map = getGlobalIDMap();
        auto it = map.find(m_nHashCode);
        if(it == end(map)) {
            std::cerr << "ID not found in global map" << std::endl;
            return "";
        }
        return (*it).second;
    }

    explicit operator bool() const {
        return *this != UNDEFINED;
    }

    friend bool operator ==(const StringID& lhs, const StringID& rhs) {
        return lhs.m_nHashCode == rhs.m_nHashCode;
    }

    friend bool operator !=(const StringID& lhs, const StringID& rhs) {
        return !(lhs == rhs);
    }

    friend bool operator <(const StringID& lhs, const StringID& rhs) {
        return lhs.m_nHashCode < rhs.m_nHashCode;
    }

    friend bool operator >(const StringID& lhs, const StringID& rhs) {
        return lhs.m_nHashCode > rhs.m_nHashCode;
    }

    friend bool operator <=(const StringID& lhs, const StringID& rhs) {
        return lhs.m_nHashCode <= rhs.m_nHashCode;
    }

    friend bool operator >=(const StringID& lhs, const StringID& rhs) {
        return lhs.m_nHashCode >= rhs.m_nHashCode;
    }

private:
    uint32_t m_nHashCode;
};

}

namespace std {
template<>
struct hash<ygg::StringID> {
public:
    size_t operator()(ygg::StringID id) const {
        return id.getHashCode();
    }
};
}

#endif // _YGG_STRINGID_HPP
