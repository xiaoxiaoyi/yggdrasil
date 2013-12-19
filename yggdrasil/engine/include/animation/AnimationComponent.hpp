#ifndef _YGG_ANIMATIONCOMPONENT_HPP_
#define _YGG_ANIMATIONCOMPONENT_HPP_

#include "core/core.hpp"
#include "core/ecs/Component.hpp"

namespace ygg {

enum class AnimationPlayState {
    LOOP,
    ONCE,
    STOP
};

struct AnimationComponent: public Component {
    static StringID getTypeName() {
        static StringID name("AnimationComponent");
        return name;
    }

    AnimationComponent() {
    }

    AnimationComponent(Entity entity, const Sprite* pSprite,
                       StringID defaultAnimation):
        m_Entity(entity),
        m_pSprite(pSprite),
        m_DefaultAnimation(defaultAnimation),
        m_bHasChangeFlag(false) {
        setAnimation(m_DefaultAnimation, AnimationPlayState::LOOP);
    }

    void setAnimation(StringID animation, AnimationPlayState state) {
        m_CurrentAnimation = m_pSprite->getAnimationIndex(animation);
        m_CurrentFrame = 0;
        m_CurrentFrameID = m_pSprite->getFrameID(m_CurrentAnimation, m_CurrentFrame);
        m_fFrameDuration = m_pSprite->getFrameDuration(m_CurrentFrameID);
        m_fFrameRelativeTime = 0.f;
        m_PlayState = state;
        m_bHasChangeFlag = true;
    }

    void update(float dt) {
        if(m_PlayState == AnimationPlayState::STOP) {
            return;
        }

        float newRelativeTime = m_fFrameRelativeTime + dt;
        float rest = newRelativeTime - m_fFrameDuration;

        if(rest > 0.f) {
            m_CurrentFrame = (m_CurrentFrame + 1) % m_pSprite->getFrameCount(m_CurrentAnimation);

            if(m_CurrentFrame == 0 && m_PlayState == AnimationPlayState::ONCE) {
                setAnimation(m_DefaultAnimation, AnimationPlayState::LOOP);
                return;
            }

            m_fFrameRelativeTime = rest;
            m_CurrentFrameID = m_pSprite->getFrameID(m_CurrentAnimation, m_CurrentFrame);
            m_fFrameDuration = m_pSprite->getFrameDuration(m_CurrentFrameID);

            m_bHasChangeFlag = true;
        }

        m_fFrameRelativeTime = newRelativeTime;
    }

    Entity m_Entity;
    const Sprite* m_pSprite;
    SpriteAnimation::Index m_CurrentAnimation;
    SpriteFrame::Index m_CurrentFrame;
    SpriteFrameID m_CurrentFrameID;
    float m_fFrameDuration;
    float m_fFrameRelativeTime;

    StringID m_DefaultAnimation;
    AnimationPlayState m_PlayState;

    bool m_bHasChangeFlag;
};

}

#endif
