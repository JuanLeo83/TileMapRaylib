#pragma once
#include <gameState/GameState.h>

class Game {
    GameState *gameState = nullptr;

public:
    Game(int width, int height, const char* title);
    ~Game();

    void run() const;

    void setGameState(GameState &gameState);
};
