#include "game/GameSystem.hpp"
#include "game/PositionComponent.hpp"
#include "game/PlayerComponent.hpp"
#include "core/SpriteComponent.hpp"

#include <boost/lexical_cast.hpp>

#include <AntTweakBar.h>

namespace ygg {

GameSystem::GameSystem(SystemInterface &systemInterface):
    m_SystemInterface(systemInterface),
    m_AssetLibrary(systemInterface.getApplicationPath() + "/data"),
    m_World([this](Entity e, const tinyxml2::XMLElement* xml){ buildEntity(e, xml); }){

    m_World.addSystem(m_RenderSystem);
    m_World.addSystem(m_PhysicSystem);
    m_World.addSystem(m_AnimationSystem);
    m_World.addSystem(m_PlayerSystem);

    m_RenderSystem.init();
    m_PhysicSystem.init(1 / 60.f);

    m_World.addEventListener(PhysicEvent::STRIKE, [this](const Event& e) {
        this->processEvent((const StrikeEvent&) e);
    });

    m_World.addEventListener(PhysicEvent::REACH_GROUND, [this](const Event& e) {
        this->processEvent((const ReachGroundEvent&) e);
    });

    m_World.addEventListener(AnimationEvent::CHANGE_FRAME, [this](const Event& e) {
        this->processEvent((const AnimationEvent&) e);
    });

    m_PlayerGroup = m_PhysicSystem.addCollisionGroup();
    m_EnemyGroup = m_PhysicSystem.addCollisionGroup();
}

void GameSystem::buildEntity(Entity e, const tinyxml2::XMLElement* xml) {
    typedef std::unordered_map<StringID, const tinyxml2::XMLElement*> XMLComponentMap;

    XMLComponentMap map;

    for(auto componentTag = xml->FirstChildElement("component"); componentTag;
        componentTag = componentTag->NextSiblingElement("component")) {
        std::string type = componentTag->Attribute("type");
        map[StringID(type)] = componentTag;
    }

    SpritePtr pSprite = nullptr;
    if(auto spriteTag = map[SpriteComponent::getTypeName()]) {
        pSprite = m_AssetLibrary.getSprite(spriteTag->Attribute("name"));
        m_World.addComponent<SpriteComponent>(e, pSprite);
        m_RenderSystem.addRenderComponent(e, pSprite);
    }

    if(auto animationTag = map[AnimationComponent::getTypeName()]) {
        if(!pSprite) {
            std::cerr << "An entity with an AnimationComponent must have a sprite" << std::endl;
        }
        m_AnimationSystem.addAnimationComponent(e, pSprite, StringID(animationTag->Attribute("animation")));
    }

    if(auto physicTag = map[PhysicComponent::getTypeName()]) {
        bool isStatic = physicTag->BoolAttribute("static");
        if(isStatic) {
            bool loop = physicTag->BoolAttribute("loop");
            std::vector<glm::vec2> vertices;
            auto verticesTag = physicTag->FirstChildElement("vertices");
            if(verticesTag) {
                for(auto vertexTag = verticesTag->FirstChildElement("vertex"); vertexTag;
                    vertexTag = vertexTag->NextSiblingElement("vertex")) {
                    vertices.emplace_back(glm::vec2 {vertexTag->FloatAttribute("x"), vertexTag->FloatAttribute("y")});
                }
                m_PhysicSystem.addStaticChain(vertices.data(), vertices.size(), loop);
            }
        } else {
            if(!pSprite) {
                std::cerr << "An entity with a non-static PhysicComponent must have a sprite" << std::endl;
            }
            glm::vec2 position { physicTag->FloatAttribute("x"), physicTag->FloatAttribute("y") };
            m_World.addComponent<PositionComponent>(e, position);
            m_PhysicSystem.addDynamicBody(e, position, pSprite, m_EnemyGroup);
        }
    }

    m_World.refresh(e);
}

void GameSystem::setMap(const std::string& mapPath) {
    auto map = m_AssetLibrary.getMap(mapPath);
    map->updateWorld(m_World, m_AssetLibrary);

    // Player entity
    m_PlayerEntity = m_World.createEntity(StringID("player"));

    SpritePtr pSprite = m_AssetLibrary.getSprite("sprite_test.xml");

    auto position = m_World.addComponent<PositionComponent>(m_PlayerEntity, glm::vec2(0, 2));
    m_World.addComponent<SpriteComponent>(m_PlayerEntity, pSprite);
    m_AnimationSystem.addAnimationComponent(m_PlayerEntity, pSprite, StringID("stand"));
    m_PhysicSystem.addDynamicBody(m_PlayerEntity, position->m_Position, pSprite, m_PlayerGroup);
    m_RenderSystem.addRenderComponent(m_PlayerEntity, pSprite);
    m_World.addComponent<PlayerComponent>(m_PlayerEntity, m_SystemInterface.getHID());

    m_World.refresh(m_PlayerEntity);
}

static bool TwEventSFML(const sf::Event* event)
{
    bool handled = false;
    TwMouseAction mouseAction;
    int key = 0;
    static int s_KMod = 0;
    static bool s_PreventTextHandling = false;
    static int s_WheelPos = 0;

    switch (event->type)
    {
    case sf::Event::KeyPressed:
        s_PreventTextHandling = false;
        s_KMod = 0;
        if (event->key.shift)   s_KMod |= TW_KMOD_SHIFT;
        if (event->key.alt)     s_KMod |= TW_KMOD_ALT;
        if (event->key.control) s_KMod |= TW_KMOD_CTRL;
        key = 0;
        switch (event->key.code)
        {
        case sf::Keyboard::Escape:
            key = TW_KEY_ESCAPE;
            break;
        case sf::Keyboard::Return:
            key = TW_KEY_RETURN;
            break;
        case sf::Keyboard::Tab:
            key = TW_KEY_TAB;
            break;
        case sf::Keyboard::BackSpace:
            key = TW_KEY_BACKSPACE;
            break;
        case sf::Keyboard::PageUp:
            key = TW_KEY_PAGE_UP;
            break;
        case sf::Keyboard::PageDown:
            key = TW_KEY_PAGE_DOWN;
            break;
        case sf::Keyboard::Up:
            key = TW_KEY_UP;
            break;
        case sf::Keyboard::Down:
            key = TW_KEY_DOWN;
            break;
        case sf::Keyboard::Left:
            key = TW_KEY_LEFT;
            break;
        case sf::Keyboard::Right:
            key = TW_KEY_RIGHT;
            break;
        case sf::Keyboard::End:
            key = TW_KEY_END;
            break;
        case sf::Keyboard::Home:
            key = TW_KEY_HOME;
            break;
        case sf::Keyboard::Insert:
            key = TW_KEY_INSERT;
            break;
        case sf::Keyboard::Delete:
            key = TW_KEY_DELETE;
            break;
        case sf::Keyboard::Space:
            key = TW_KEY_SPACE;
            break;
        default:
            if (event->key.code >= sf::Keyboard::F1 && event->key.code <= sf::Keyboard::F15)
                key = TW_KEY_F1 + event->key.code - sf::Keyboard::F1;
            else if (s_KMod & TW_KMOD_ALT)
            {
                if (event->key.code >= sf::Keyboard::A && event->key.code <= sf::Keyboard::Z)
                {
                    if (s_KMod & TW_KMOD_SHIFT)
                        key = 'A' + event->key.code - sf::Keyboard::A;
                    else
                        key = 'a' + event->key.code - sf::Keyboard::A;
                }
            }
        }
        if (key != 0)
        {
            handled = TwKeyPressed(key, s_KMod);
            s_PreventTextHandling = true;
        }
        break;
    case sf::Event::KeyReleased:
        s_PreventTextHandling = false;
        s_KMod = 0;
        break;
    case sf::Event::TextEntered:
        if (!s_PreventTextHandling && event->text.unicode != 0 && (event->text.unicode & 0xFF00) == 0)
        {
            if ((event->text.unicode & 0xFF) < 32) // CTRL+letter
                handled = TwKeyPressed((event->text.unicode & 0xFF)+'a'-1, TW_KMOD_CTRL|s_KMod);
            else
                handled = TwKeyPressed(event->text.unicode & 0xFF, 0);
        }
        s_PreventTextHandling = false;
        break;
    case sf::Event::MouseMoved:
        handled = TwMouseMotion(event->mouseMove.x, event->mouseMove.y);
        break;
    case sf::Event::MouseButtonPressed:
    case sf::Event::MouseButtonReleased:
        mouseAction = (event->type==sf::Event::MouseButtonPressed) ? TW_MOUSE_PRESSED : TW_MOUSE_RELEASED;
        switch (event->mouseButton.button)
        {
        case sf::Mouse::Left:
            handled = TwMouseButton(mouseAction, TW_MOUSE_LEFT);
            break;
        case sf::Mouse::Middle:
            handled = TwMouseButton(mouseAction, TW_MOUSE_MIDDLE);
            break;
        case sf::Mouse::Right:
            handled = TwMouseButton(mouseAction, TW_MOUSE_RIGHT);
            break;
        default:
            break;
        }
        break;
    case sf::Event::MouseWheelMoved:
        s_WheelPos += event->mouseWheel.delta;
        handled = TwMouseWheel(s_WheelPos);
        break;
    default:
        break;
    }

    return handled;
}

void GameSystem::handleEvent(const sf::Event& e) {
    TwEventSFML(&e);
}

void GameSystem::processEvent(const ReachGroundEvent& e) {
    AnimationComponent* pAnimationComponent = m_World.getComponent<AnimationComponent>(e.m_Entity);
    if(pAnimationComponent) {
        pAnimationComponent->setAnimation(StringID("stand"), AnimationPlayState::LOOP);
    }
    std::clog << "Reach Ground" << std::endl;
}

void GameSystem::processEvent(const StrikeEvent& e) {
    static uint32_t i = 0;
    std::clog << "Aie !" << i++ << std::endl;
}

void GameSystem::processEvent(const AnimationEvent& e) {
    PhysicComponent* pPhysicComponent = m_World.getComponent<PhysicComponent>(e.m_Entity);
    if(pPhysicComponent) {
        pPhysicComponent->setFrame(e.m_FrameID);
    }
    RenderComponent* pRenderComponent = m_World.getComponent<RenderComponent>(e.m_Entity);
    if(pRenderComponent) {
        pRenderComponent->setFrame(e.m_FrameID);
    }
}

void GameSystem::runGameIteration(double elapsedTime) {
    size_t w, h;
    m_SystemInterface.getWindowSize(w, h);
    float ratio = (float) w / h;

    m_World.loopStart();

    PhysicComponent* pPlayerPhysicComponent = m_World.getComponent<PhysicComponent>(m_PlayerEntity);

    // Rendering
    m_RenderSystem.startFrame(
                View(pPlayerPhysicComponent->getPosition(),
                     glm::vec2(ratio * 2.5f, 2.5f)));

    m_RenderSystem.renderAllComponents();

    DebugRenderer::getInstance()->startGameIteration(w, h, m_RenderSystem);

    DebugRenderer::getInstance()->displayEntityData(m_PlayerEntity, m_World);

    m_AnimationSystem.update(elapsedTime);

    // Update the physic of the world
    m_PhysicSystem.update(elapsedTime);

    // Control player
    m_PlayerSystem.update(elapsedTime);

    m_PhysicSystem.renderDebug();

    m_RenderSystem.endFrame();

    DebugRenderer::getInstance()->endGameIteration();
}

}
