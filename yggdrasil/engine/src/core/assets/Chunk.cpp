#include "core/assets/Chunk.hpp"
#include "core/assets/AssetLibrary.hpp"
#include "core/ecs/World.hpp"

namespace ygg {

Chunk::Chunk(const std::string& path) {
    auto error = m_XML.LoadFile(path.c_str());
    if(error) {
        std::cerr << "Unable to load chunk " << path << "(" << error << ")" << std::endl;
    }
}

void Chunk::updateWorld(World &world, AssetLibrary &assetLibrary) {
    auto chunkTag = m_XML.FirstChildElement();
    for(auto entityTag = chunkTag->FirstChildElement("entity"); entityTag;
        entityTag = entityTag->NextSiblingElement("entity")) {
        world.createEntity(entityTag);
    }
}

}
