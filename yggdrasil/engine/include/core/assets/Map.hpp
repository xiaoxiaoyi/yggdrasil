#ifndef _YGG_MAP_HPP
#define _YGG_MAP_HPP

#include <tinyxml2.h>
#include <vector>

namespace ygg {

class World;
class AssetLibrary;
class Chunk;

class Map {
public:
    Map(const tinyxml2::XMLDocument& xml, AssetLibrary& assetLibrary);

    void updateWorld(World& world, AssetLibrary& assetLibrary);

private:
    std::vector<Chunk*> m_Chunks;
};

}

#endif // _YGG_MAP_HPP
