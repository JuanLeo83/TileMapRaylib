#include <iostream>

#include "engine/game/Game.h"

int main() {
    auto game = Game(800, 600, "Tileset Editor");
    auto gameState = GameState();
    game.setGameState(gameState);

    game.run();

    return 0;
}
