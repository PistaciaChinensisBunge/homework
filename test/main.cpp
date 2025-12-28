#include "Game.h"
#include <iostream>

int main() {
    Game game;

    if (!game.init("resources/maze20x20.txt", "resources/")) {
        std::cerr << "Failed to initialize game" << std::endl;
        return -1;
    }

    game.run();

    return 0;
}
