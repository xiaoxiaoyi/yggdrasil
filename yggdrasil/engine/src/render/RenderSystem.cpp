// Yggdrasil Project (2013)
// Laurent "Otium" Noel | Vincent "Hekiat" Lesueur 
// laurent.noel.c2ba[at]gmail.com | lesueurv[at]gmail.com

#include "core/core.hpp"
#include "core/ecs/World.hpp"

#include <glm/gtc/type_ptr.hpp>

#include "render/RenderSystem.hpp"
#include "render/shaders/sprite_shaders.hpp"

#include "game/PositionComponent.hpp"

namespace ygg {

void RenderSystem::init() {
	m_Quad.init();
    if(!gloops::buildProgram(SPRITE_VERTEX_SHADER_SRC, SPRITE_FRAGMENT_SHADER_SRC, m_SpriteProgram)) {
        log(LogLevel::ERROR, "Fail to build sprite program for GLSL.");
    }
    m_uProjectionMatrix = gloops::Uniform(m_SpriteProgram, SHADER_UNIFORM_PROJECTIONMATRIX);
    m_uMVMatrix = gloops::Uniform(m_SpriteProgram, SHADER_UNIFORM_MVMATRIX);
    m_uSpriteTexture = gloops::Uniform(m_SpriteProgram, "uSpriteTexture");
    m_uTexCoordsTransform = gloops::Uniform(m_SpriteProgram, "uTexCoordsTransform");
}

void RenderSystem::startFrame(const View& view) {
    glClear(GL_COLOR_BUFFER_BIT);
    m_View = view;
}

const View& RenderSystem::getView() const {
    return m_View;
}

void RenderSystem::endFrame() {
	
}

void RenderSystem::renderAllComponents() {
    m_SpriteProgram.use();

    m_uProjectionMatrix.set(m_View.getProjectionMatrix());

    glm::mat3 viewMatrix = m_View.getViewMatrix();

    m_uSpriteTexture.set(0);
    glActiveTexture(GL_TEXTURE0);

    m_Quad.bindVAO();

    for(size_t i = 0; i < _entities.size(); ++i) {
        Entity entity = _entities[i];

        RenderComponent* pRendererComponent = _world->getComponent<RenderComponent>(entity);

        glBindTexture(GL_TEXTURE_2D, pRendererComponent->m_TextureObject);
        const SpriteFrame& frame = pRendererComponent->m_pSprite->getFrame(pRendererComponent->m_CurrentFrame);

        float w = pRendererComponent->m_pSprite->getImage()->getWidth();
        float h = pRendererComponent->m_pSprite->getImage()->getHeight();

        glm::vec4 texcoordsTransform(frame.m_nX / w, frame.m_nY / h,
                                     frame.m_nWidth / w, frame.m_nHeight / h);

        float ratio = (float) frame.m_nWidth / frame.m_nHeight;
        float height = 0.4;

        m_uTexCoordsTransform.set(texcoordsTransform);

        glm::mat3 scale(ratio * height, 0, 0,
                        0, height, 0,
                        0, 0, 1);

        glm::mat3 translate(1, 0, 0,
                            0, 1, 0,
                            0.5f - frame.m_Center.x, -0.5 + frame.m_Center.y, 1);

        glm::vec2 position = _world->getComponent<PositionComponent>(entity)->m_Position;

        glm::mat3 transform = glm::mat3(1, 0, 0,
                                        0, 1, 0,
                                        position.x, position.y, 1)
                * scale * translate;

        m_uMVMatrix.set(viewMatrix * transform);
        m_Quad.render();
    }

    glBindVertexArray(0);
    glBindTexture(GL_TEXTURE_2D, 0);
}

gloops::TextureObject& RenderSystem::getTexture(const Image* pImage) {
    auto it = m_TextureMap.find(pImage);
    if(it != std::end(m_TextureMap)) {
        return (*it).second;
    }
    auto& texObject = m_TextureMap[pImage];

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texObject.glId());
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pImage->getWidth(), pImage->getHeight(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pImage->getPixels());

    return texObject;
}

void RenderSystem::addRenderComponent(Entity entity, const Sprite* pSprite) {
    auto& texObject = getTexture(pSprite->getImage());
    _world->addComponent<RenderComponent>(entity, texObject.glId(), pSprite, 0);
}

void RenderSystem::onRegistered() {
    registerComponent<RenderComponent>();
    registerComponent<PositionComponent>();
}

void RenderSystem::process(float dt, Entity e) {

}

}
