#ifndef _YGG_SPRITEDATA_HPP
#define _YGG_SPRITEDATA_HPP

#include "../StringID.hpp"
#include "../Log.hpp"

#include "Image.hpp"

#include <vector>
#include <glm/glm.hpp>
#include <tinyxml2.h>

namespace ygg {

struct SpriteBox {
    //! Predefined box types
    static StringID STRIKE;
    static StringID HURT;
    static StringID BODY;

    StringID m_Type;

    //! Position of the vertices of the boxes relative to
    //! the center of the frame
    glm::vec2 m_Vertices[4];

    SpriteBox() {
    }

    SpriteBox(StringID type, std::initializer_list<glm::vec2> vertices):
        m_Type(type) {
        std::copy(begin(vertices), end(vertices), m_Vertices);
    }
};

typedef std::vector<SpriteBox> SpriteBoxArray;

struct SpriteFrame {
    typedef uint16_t Index;

    //! Duration of the frame in seconds.
    float m_fDuration;

    //! Position of the top-left corner of the frame in the image.
    uint32_t m_nX, m_nY;
    //! Dimensions of the frame in pixels.
    uint32_t m_nWidth, m_nHeight;

    //! Position of the center of the sprite in frame space
    glm::vec2 m_Center;

    //! Array of boxes that define the physic behavior of an instance of the sprite
    //! during this frame.
    SpriteBoxArray m_Boxes;

    SpriteFrame() {
    }

    SpriteFrame(float duration, const SpriteBoxArray& boxes):
        m_fDuration(duration), m_Boxes(boxes) {
    }
};

typedef uint32_t SpriteFrameID;
typedef uint16_t SpriteFrameCount;

struct SpriteAnimation {
    typedef uint16_t Index;
    static const Index NO_INDEX = std::numeric_limits<uint16_t>::max();

    StringID m_Name;
    SpriteFrame::Index m_nFrameOffset;
    SpriteFrameCount m_nFrameCount;

    SpriteAnimation(StringID name, SpriteFrame::Index frameOffset, SpriteFrameCount frameCount):
        m_Name(name), m_nFrameOffset(frameOffset), m_nFrameCount(frameCount) {
    }
};

class AssetLibrary;

class Sprite {
    void addFrame(const SpriteFrame& frame) {
        m_Frames.emplace_back(frame);
    }

    void addAnimation(const SpriteAnimation& animation) {
        m_AnimationMap[animation.m_Name] = m_Animations.size();
        m_Animations.emplace_back(animation);
    }

public:
    Sprite(): m_pImage(nullptr) {
    }

    Sprite(const tinyxml2::XMLDocument& xml, AssetLibrary& library);

    void addAnimation(StringID name, const std::vector<SpriteFrame>& frames) {
        addAnimation(SpriteAnimation(name, m_Frames.size(), frames.size()));
        for(const auto& frame: frames) {
            addFrame(frame);
        }
    }

    const SpriteBoxArray& getBoxes(SpriteFrameID frame) const {
        return m_Frames[frame].m_Boxes;
    }

    SpriteAnimation::Index getAnimationIndex(StringID name) const {
        auto it = m_AnimationMap.find(name);
        if(it == end(m_AnimationMap)) {
            log(LogLevel::WARNING, str("Animation ", name.toString(), " not found in the sprite"));
            return SpriteAnimation::NO_INDEX;
        }
        return (*it).second;
    }

    SpriteFrameID getFrameID(SpriteAnimation::Index animation, SpriteFrame::Index frame) const {
        return m_Animations[animation].m_nFrameOffset + frame;
    }

    float getFrameDuration(SpriteFrameID frame) const {
        return m_Frames[frame].m_fDuration;
    }

    SpriteFrameCount getFrameCount(SpriteAnimation::Index animation) const {
        return m_Animations[animation].m_nFrameCount;
    }

    const SpriteFrame& getFrame(SpriteFrameID frame) const {
        assert(frame < m_Frames.size());
        return m_Frames[frame];
    }

    const Image* getImage() const {
        return m_pImage;
    }

private:
    std::vector<SpriteFrame> m_Frames;
    std::vector<SpriteAnimation> m_Animations;
    std::unordered_map<StringID, SpriteAnimation::Index> m_AnimationMap;
    Image* m_pImage;
};

}

#endif // _YGG_SPRITEDATA_HPP
