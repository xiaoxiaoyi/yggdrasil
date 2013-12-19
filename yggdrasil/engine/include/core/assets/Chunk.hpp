#ifndef _YGG_CHUNK_HPP
#define _YGG_CHUNK_HPP

#include <string>
#include <tinyxml2.h>

namespace ygg {

class AssetLibrary;
class World;

class Chunk {
public:
    Chunk(const std::string& path);

    void updateWorld(World& world, AssetLibrary& assetLibrary);

private:
    tinyxml2::XMLDocument m_XML;
};

}

#endif // _YGG_CHUNK_HPP
