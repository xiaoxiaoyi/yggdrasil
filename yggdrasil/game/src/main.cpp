#include <iostream>
#include <cstdlib>

#define GLEW_BUILD GLEW_STATIC

#include <SFML/Window.hpp>
#include <GL/glew.h>

#include "game/GameSystem.hpp"
#include "SFMLSystemInterface.hpp"

#include "core/core.hpp"

using namespace ygg;

void testObjectManager() {
    ObjectManager<int> manager;

    std::vector<ObjectID> ids;

    static const int nb = 1024;

    for(int i = 0; i < nb; ++i) {
        ids.push_back(manager.add(i));
    }

    bool check_array[2 * nb];
    std::fill(std::begin(check_array), std::end(check_array), true);

    srand(0);
    for(int i = 0; i < nb / 2; ++i) {
        int idx = rand() % nb;

        if(manager.isValid(ids[idx])) {
            int value = manager[ids[idx]];
            check_array[value] = false;
        }
        manager.remove(ids[idx]);

    }

    for(int i = nb; i < 2 * nb; ++i) {
        ids.push_back(manager.add(i));
    }

    for(const auto& data: manager) {
        if(!check_array[data]) {
            return;
        }
    }
}

int main(int argc, char** argv) {
	std::string pathToApp(argv[0]);

	size_t index = pathToApp.find_last_of("/");
	if(index == std::string::npos) {
        index = pathToApp.find_last_of("\\");
	}
	std::string path = pathToApp.substr(0, index);

    sf::Window window(sf::VideoMode(1024, 512), "Yggdrasil");
    window.setVerticalSyncEnabled(true);

    GLenum glewStatus;
    if(GLEW_OK != (glewStatus = glewInit())) {
        std::cerr << glewGetErrorString(glewStatus) << std::endl;
        return EXIT_FAILURE;
    }

    SFMLSystemInterface systemInterface(window, path);

    GameSystem gameSystem(systemInterface);

    if(argc == 1) {
        gameSystem.setMap("test.map.xml");
    } else {
        gameSystem.setMap(argv[1]);
    }

    sf::Clock clock;
    double frameTime = 0;
    uint32_t frameCount = 0;

    while(window.isOpen()) {
        double startTime = clock.getElapsedTime().asSeconds();

        gameSystem.runGameIteration(frameTime);

        window.display();

        sf::Event e;
        while(window.pollEvent(e)) {
            if(e.type == sf::Event::Closed) {
                window.close();
            }
            gameSystem.handleEvent(e);
        }

        frameTime = clock.getElapsedTime().asSeconds() - startTime;
        ++frameCount;
    }

    return EXIT_SUCCESS;
}
