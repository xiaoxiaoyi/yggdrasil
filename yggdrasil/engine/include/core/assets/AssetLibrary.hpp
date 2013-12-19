#ifndef _YGG_ASSETLIBRARY_HPP
#define _YGG_ASSETLIBRARY_HPP

#include <string>
#include <unordered_map>
#include <memory>

#include "../StringID.hpp"

#include "Sprite.hpp"
#include "Image.hpp"
#include "Map.hpp"
#include "Chunk.hpp"

namespace ygg {

typedef Image* ImagePtr;
typedef Sprite* SpritePtr;
typedef Map* MapPtr;
typedef Chunk* ChunkPtr;

class AssetLibrary {
public:
    AssetLibrary(const std::string& path):
        m_Path(path) {
        if(m_Path.back() != '/') {
            m_Path.push_back('/');
        }
    }

    ImagePtr getImage(const std::string& path);

    SpritePtr getSprite(const std::string& path);

    MapPtr getMap(const std::string& path);

    ChunkPtr getChunk(const std::string& path);

private:
    typedef std::unordered_map<StringID, std::unique_ptr<Image>> ImageMap;
    typedef std::unordered_map<StringID, std::unique_ptr<Sprite>> SpriteMap;
    typedef std::unordered_map<StringID, std::unique_ptr<Map>> MapMap;
    typedef std::unordered_map<StringID, std::unique_ptr<Chunk>> ChunkMap;

    std::string m_Path;

    ImageMap m_Images;
    SpriteMap m_Sprites;
    MapMap m_Maps;
    ChunkMap m_Chunks;
};

}

#endif // _YGG_ASSETLIBRARY_HPP
