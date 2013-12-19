#include "core/assets/Sprite.hpp"
#include "core/assets/AssetLibrary.hpp"

namespace ygg {

StringID SpriteBox::STRIKE("strike");
StringID SpriteBox::HURT("hurt");
StringID SpriteBox::BODY("body");

Sprite::Sprite(const tinyxml2::XMLDocument& xml, AssetLibrary& library):
    m_pImage(nullptr) {
    typedef const tinyxml2::XMLElement XMLElement;

    XMLElement* spriteTag = xml.FirstChildElement();

    std::string imagePath = spriteTag->Attribute("image");
    m_pImage = library.getImage(imagePath);

    std::string transparentColorStr = spriteTag->Attribute("transparentColor");
    uint32_t r, g, b;
    std::stringstream ss;
    ss << transparentColorStr;
    ss >> r >> g >> b;
    m_pImage->setTransparentColor(r, g, b);

    for(XMLElement* animationTag = spriteTag->FirstChildElement(); animationTag;
        animationTag = animationTag->NextSiblingElement()) {

        std::vector<SpriteFrame> frames;

        for(XMLElement* frameTag = animationTag->FirstChildElement(); frameTag;
            frameTag = frameTag->NextSiblingElement()) {

            frames.emplace_back();

            frames.back().m_fDuration = frameTag->FloatAttribute("duration");
            frames.back().m_nX = frameTag->IntAttribute("x");
            frames.back().m_nY = frameTag->IntAttribute("y");
            frames.back().m_nWidth = frameTag->IntAttribute("w");
            frames.back().m_nHeight = frameTag->IntAttribute("h");
            frames.back().m_Center.x = frameTag->FloatAttribute("centerX");
            frames.back().m_Center.y = frameTag->FloatAttribute("centerY");

            for(XMLElement* boxTag = frameTag->FirstChildElement(); boxTag;
                boxTag = boxTag->NextSiblingElement()) {

                SpriteBox box;
                box.m_Type = StringID(boxTag->Attribute("type"));
                XMLElement* vertexTag = boxTag->FirstChildElement();
                for(uint8_t i = 0; i < 4; ++i) {
                    if(!vertexTag) {
                        log(LogLevel::ERROR, "Not enough vertices defined for a box");
                    }
                    box.m_Vertices[i] = glm::vec2(vertexTag->FloatAttribute("x"), vertexTag->FloatAttribute("y"));
                    vertexTag = vertexTag->NextSiblingElement();
                }

                frames.back().m_Boxes.emplace_back(box);
            }
        }

        addAnimation(StringID(animationTag->Attribute("name")), frames);
    }
}

}
