#include <iostream>

#include "rlImGui.h"
#include "engine/game/Game.h"

int main() {
    rlImGuiSetup(true);
    auto game = Game(800, 600, "Tileset Editor");
    auto gameState = GameState();
    game.setGameState(gameState);

    game.run();

    return 0;
}
