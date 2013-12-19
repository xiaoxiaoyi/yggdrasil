#ifndef _YGG_SPRITECOMPONENT_HPP
#define _YGG_SPRITECOMPONENT_HPP

#include "core/ecs/Component.hpp"

namespace ygg {

class Sprite;
struct SpriteComponent: public Component {
    static StringID getTypeName() {
        static StringID name("SpriteComponent");
        return name;
    }

    virtual void display(TwBar *bar) {
        TwAddButton(bar, "SpriteComponent", nullptr, nullptr, "");
    }

    SpriteComponent():
        m_pSprite(nullptr) {

    }

    SpriteComponent(const Sprite* pSprite):
        m_pSprite(pSprite) {
    }

    virtual void init(const tinyxml2::XMLElement* xml) {
    }

    const Sprite* m_pSprite;
};

}

#endif // _YGG_SPRITECOMPONENT_HPP
