#ifndef _YGG_RENDERCOMPONENT_HPP_
#define _YGG_RENDERCOMPONENT_HPP_

#include <glm/glm.hpp>
#include "core/ecs/Component.hpp"

namespace ygg {

struct RenderComponent: public Component {
    static StringID getTypeName() {
        static StringID name("RenderComponent");
        return name;
    }

    RenderComponent() {
    }

    RenderComponent(GLuint textureObject, const Sprite* pSprite, SpriteFrameID currentFrame):
        m_TextureObject(textureObject), m_pSprite(pSprite), m_CurrentFrame(currentFrame) {
    }

    void setFrame(SpriteFrameID frameID) {
        m_CurrentFrame = frameID;
    }

    virtual void display(TwBar *bar) {
        TwAddButton(bar, "RenderComponent", nullptr, nullptr, "");
        TwAddVarRO(bar, "RenderComponent::textureObject", TW_TYPE_UINT32, &m_TextureObject, " label=textureObject ");
        TwAddVarRO(bar, "RenderComponent::currentFrame", TW_TYPE_UINT32, &m_CurrentFrame, " label=currentFrame ");
    }

    GLuint m_TextureObject;
    const Sprite* m_pSprite;
    SpriteFrameID m_CurrentFrame;
};

}

#endif // _YGG_RENDERCOMPONENT_HPP
