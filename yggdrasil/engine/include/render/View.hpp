#ifndef _YGG_VIEW_HPP
#define _YGG_VIEW_HPP

#include "core/core.hpp"

namespace ygg {

class View {
public:
    glm::vec2 m_Center; // Center of the view in world space
    glm::vec2 m_HalfSize; // Half size of the view in world space

    View(const glm::vec2& center = glm::vec2(0, 0), const glm::vec2& halfSize = glm::vec2(0, 0)): m_Center(center), m_HalfSize(halfSize) {
    }

    glm::mat4 getProjectionMatrix() const {
        return glm::ortho(-m_HalfSize.x, m_HalfSize.x, -m_HalfSize.y, m_HalfSize.y);
    }

    glm::mat3 getViewMatrix() const {
        return glm::mat3(1, 0, 0,
                         0, 1, 0,
                         -m_Center.x, -m_Center.y, 1);
    }
};

}

#endif // _YGG_VIEW_HPP
