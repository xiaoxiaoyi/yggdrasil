#ifndef _YGG_SFMLHUMANINTERFACEDEVICE_HPP
#define _YGG_SFMLHUMANINTERFACEDEVICE_HPP

#include "core/core.hpp"
#include <SFML/Window.hpp>

namespace ygg {

class SFMLHumanInterfaceDevice: public HumanInterfaceDevice {
public:
    SFMLHumanInterfaceDevice() {
    }

protected:
    virtual uint32_t getButtonStates() {
        return (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) << UP_OFFSET) |
        (sf::Keyboard::isKeyPressed(sf::Keyboard::S) << DOWN_OFFSET) |
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) << LEFT_OFFSET) |
        (sf::Keyboard::isKeyPressed(sf::Keyboard::D) << RIGHT_OFFSET) |
        (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) << BUTTON0_OFFSET);
    }
};

}

#endif // _YGG_SFMLHUMANINTERFACEDEVICE_HPP
