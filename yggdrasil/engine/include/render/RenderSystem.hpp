#ifndef _YGG_RENDERSYSTEM_HPP_
#define _YGG_RENDERSYSTEM_HPP_

#define GLOOPS_USE_GLM
#include <gloops/gloops.hpp>

#include "core/core.hpp"
#include "core/ecs/systems/EntityProcessingSystem.hpp"

#include "Quad.hpp"
#include "RenderComponent.hpp"
#include "View.hpp"

namespace ygg {

class RenderSystem: public EntityProcessingSystem {
public:
    static StringID getTypeName() {
        static StringID name("RenderSystem");
        return name;
    }

    void init();

    void startFrame(const View& view);
    
    const View& getView() const;

    void endFrame();

    void renderAllComponents();

    void addRenderComponent(Entity entity, const Sprite* pSprite);

    virtual void process(float dt, Entity e);

protected:
    virtual void onRegistered();

private:
    gloops::TextureObject& getTexture(const Image* pImage);

    typedef std::unordered_map<const Image*, gloops::TextureObject> TextureMap;
    TextureMap m_TextureMap;

    View m_View;

    Quad m_Quad;
    gloops::Program m_SpriteProgram;
    gloops::Uniform m_uProjectionMatrix;
    gloops::Uniform m_uMVMatrix;
    gloops::Uniform m_uSpriteTexture;
    gloops::Uniform m_uTexCoordsTransform;
};

}

#endif

