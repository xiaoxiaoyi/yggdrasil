#ifndef _YGG_DEBUGRENDERER_HPP
#define _YGG_DEBUGRENDERER_HPP

#include <glm/glm.hpp>
#include "RenderSystem.hpp"

#include "core/ecs/World.hpp"

namespace ygg {

/**
 * @brief The DebugRenderer is a singleton class that can be used from anywhere in the
 * renderer during a game loop iteration to easily draw debug features.
 *
 * It must be used exclusively for developpment facilities and not to display any feature
 * in the shipping version of the game.
 */
class DebugRenderer {
public:
    /**
     * @brief getInstance must be used to access to the instance of the DebugRenderer.
     * @return The instance of the DebugRenderer.
     */
    static DebugRenderer* getInstance();

    /**
     * @brief DebugRenderer Constructor. Creation of more that one DebugRenderer instance creates a disturbance
     * in the force.
     */
    DebugRenderer();

    /**
     * @brief startFrame This method is called at the beginning of the game iteration. It uses the parameters of
     * of the render system to set the camera.
     * @param renderSystem
     */
    void startGameIteration(size_t windowWidth, size_t windowHeight, const RenderSystem& renderSystem);

    void endGameIteration();

    /**
     * @brief drawLines Draw a set of lines
     * @param vertices A pointer to the array of vertices
     * @param vertexCount The number of vertices
     * @param color The color of the lines
     * @param transform The transform to apply to the lines
     */
    void drawLines(const glm::vec2* vertices, size_t vertexCount,
                   const glm::vec3& color, const glm::mat3& transform);

    void displayEntityData(Entity entity, World& world);

private:
    static DebugRenderer* s_Instance;

    DebugRenderer(const DebugRenderer&) = delete;

    DebugRenderer& operator =(const DebugRenderer&) = delete;

    gloops::Program m_Program;
    gloops::BufferObject m_VBO;
    gloops::VertexArrayObject m_VAO;

    View m_View;

    TwBar* m_pEntityBar;
    Entity m_CurrentDisplayedEntity;
    std::string m_EntityName;
};

}

#endif // _YGG_DEBUGRENDERER_HPP
