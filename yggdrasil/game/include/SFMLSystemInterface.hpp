#ifndef _YGG_SFMLSYSTEMINTERFACE_HPP
#define _YGG_SFMLSYSTEMINTERFACE_HPP

#include "core/core.hpp"
#include "SFMLHumanInterfaceDevice.hpp"

#include <SFML/Graphics.hpp>

namespace ygg {

class SFMLSystemInterface: public SystemInterface {
public:
    SFMLHumanInterfaceDevice m_HID;
    sf::Window& m_Window;
    std::string m_ApplicationPath;

    SFMLSystemInterface(sf::Window& window, const std::string& applicationPath):
        m_Window(window), m_ApplicationPath(applicationPath) {
    }

    virtual HumanInterfaceDevice& getHID() {
        return m_HID;
    }

    virtual bool loadImage(const char* path, Image& image) {
        sf::Image img;
        if(!img.loadFromFile(path)) {
            return false;
        }
        image = Image(img.getSize().x, img.getSize().y, img.getPixelsPtr());
        return true;
    }

    virtual std::string getApplicationPath() const {
        return m_ApplicationPath;
    }

    virtual void getWindowSize(size_t& w, size_t& h) const {
        w = m_Window.getSize().x;
        h = m_Window.getSize().y;
    }
};

}

#endif
