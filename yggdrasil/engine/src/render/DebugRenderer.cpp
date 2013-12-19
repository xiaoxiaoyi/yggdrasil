#include "render/DebugRenderer.hpp"
#include "render/shaders/debug_shaders.hpp"

#include <stdexcept>

#include <AntTweakBar.h>

namespace ygg {

DebugRenderer* DebugRenderer::s_Instance = nullptr;

static void CopyStdStringToClient(std::string& destinationClientString, const std::string& sourceLibraryString)
{
  // Copy the content of souceString handled by the AntTweakBar library to destinationClientString handled by your application
  destinationClientString = sourceLibraryString;
}

DebugRenderer::DebugRenderer(): m_pEntityBar(nullptr) {
    if(s_Instance) {
        throw std::runtime_error("Cannot create two instance DebugRenderer");
    }

    gloops::buildProgram(DEBUG_VERTEX_SHADER_SRC, DEBUG_FRAGMENT_SHADER_SRC, m_Program);
    gloops::BufferBind vbo(GL_ARRAY_BUFFER, m_VBO);
    gloops::VertexArrayBind vao(m_VAO);
    vao.enableVertexAttribArray(SHADER_ATTR_POSITION);
    vao.vertexAttribPointer(
        SHADER_ATTR_POSITION,
        2,
        GL_FLOAT,
        GL_FALSE,
        0,
        0
    );

    s_Instance = this;

    TwInit(TW_OPENGL_CORE, nullptr);
    TwCopyStdStringToClientFunc(CopyStdStringToClient);

    m_pEntityBar = TwNewBar("Entity");
}

DebugRenderer* DebugRenderer::getInstance() {
    assert(s_Instance);
    return s_Instance;
}

/**
 * @brief startFrame This method is called at the beginning of the game iteration. It uses the parameters of
 * of the render system to set the camera.
 * @param renderSystem
 */
void DebugRenderer::startGameIteration(size_t windowWidth, size_t windowHeight, const RenderSystem& renderSystem) {
    TwWindowSize(windowWidth, windowHeight);

    m_Program.use();

    m_View = renderSystem.getView();

    gloops::Uniform uProjectionMatrix(m_Program, SHADER_UNIFORM_PROJECTIONMATRIX);
    uProjectionMatrix.set(m_View.getProjectionMatrix());
}

void DebugRenderer::endGameIteration() {
    TwDraw();
}

/**
 * @brief drawLines Draw a set of lines
 * @param vertices A pointer to the array of vertices
 * @param vertexCount The number of vertices
 * @param color The color of the lines
 * @param transform The transform to apply to the lines
 */
void DebugRenderer::drawLines(const glm::vec2* vertices, size_t vertexCount,
                              const glm::vec3& color, const glm::mat3& transform) {
    gloops::BufferBind vbo(GL_ARRAY_BUFFER, m_VBO);
    vbo.bufferData(vertexCount, vertices, GL_DYNAMIC_DRAW);

    gloops::Uniform uMVMatrix(m_Program, SHADER_UNIFORM_MVMATRIX);
    uMVMatrix.set(m_View.getViewMatrix() * transform);

    gloops::Uniform uColor(m_Program, SHADER_UNIFORM_COLOR);
    uColor.set(color);

    gloops::VertexArrayBind(m_VAO).drawArrays(GL_LINES, 0, vertexCount);
}

void DebugRenderer::displayEntityData(Entity entity, World& world) {
    TwRemoveAllVars(m_pEntityBar);

    m_CurrentDisplayedEntity = entity;
    m_EntityName = world.getEntityInfo(entity).getName().toString();

    TwAddVarRO(m_pEntityBar, "id", TW_TYPE_UINT32, &m_CurrentDisplayedEntity._id, "");
    TwAddVarRO(m_pEntityBar, "name", TW_TYPE_STDSTRING, &m_EntityName, "");

    TwAddSeparator(m_pEntityBar, "sep0", "");

    auto components = world.getComponents(entity);
    for(auto pComponent: components) {
        pComponent->display(m_pEntityBar);
    }
}

}
