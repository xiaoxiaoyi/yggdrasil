#include "core/assets/Map.hpp"
#include "core/assets/AssetLibrary.hpp"
#include "core/assets/Chunk.hpp"

namespace ygg {

Map::Map(const tinyxml2::XMLDocument& xml, AssetLibrary& assetLibrary) {
    auto mapTag = xml.FirstChildElement();
    // Load all the chunks of the map
    for(auto chunkTag = mapTag->FirstChildElement("chunk"); chunkTag;
        chunkTag = chunkTag->NextSiblingElement("chunk")) {
        auto chunkName = chunkTag->Attribute("name");
        m_Chunks.emplace_back(assetLibrary.getChunk(chunkName));
   }
}

void Map::updateWorld(World& world, AssetLibrary& assetLibrary) {
    for(auto pChunk: m_Chunks) {
        pChunk->updateWorld(world, assetLibrary);
    }
}

}
