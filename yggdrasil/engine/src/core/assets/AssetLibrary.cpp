#include "core/assets/AssetLibrary.hpp"
#include <SFML/Graphics.hpp>

namespace ygg {

ImagePtr AssetLibrary::getImage(const std::string& path) {
    StringID id(path);
    auto& ptr = m_Images[id];
    if(!ptr) {
        sf::Image img;
        if(!img.loadFromFile(m_Path + "images/" + path)) {
            std::cerr << "Unable to load image " << path << std::endl;
            return nullptr;
        }
        ptr.reset(new Image{img.getSize().x, img.getSize().y, img.getPixelsPtr()});
    }
    return ptr.get();
}

SpritePtr AssetLibrary::getSprite(const std::string& path) {
    StringID id(path);
    auto& ptr = m_Sprites[id];
    if(!ptr) {
        auto completePath = m_Path + "sprites/" + path;
        tinyxml2::XMLDocument doc;
        auto error = doc.LoadFile(completePath.c_str());
        if(error) {
            std::cerr << "Unable to load sprite " << path << "(" << error << ")" << std::endl;
            return nullptr;
        }
        ptr.reset(new Sprite{doc, *this});
    }
    return ptr.get();
}

MapPtr AssetLibrary::getMap(const std::string& path) {
    StringID id(path);
    auto& ptr = m_Maps[id];
    if(!ptr) {
        auto completePath = m_Path + "maps/" + path;
        tinyxml2::XMLDocument doc;
        auto error = doc.LoadFile(completePath.c_str());
        if(error) {
            std::cerr << "Unable to load map " << path << "(" << error << ")" << std::endl;
            return nullptr;
        }
        ptr.reset(new Map{doc, *this});
    }
    return ptr.get();
}

ChunkPtr AssetLibrary::getChunk(const std::string& path) {
    StringID id(path);
    auto& ptr = m_Chunks[id];
    if(!ptr) {
        auto completePath = m_Path + "chunks/" + path;
        ptr.reset(new Chunk{completePath});
    }
    return ptr.get();
}

}
