#include <raylib.h>
#include "Game.h"

Game::Game(const int width, const int height, const char* title) {
    InitWindow(width, height, title);
    SetTargetFPS(60);

    InitAudioDevice();
}

Game::~Game() {
    CloseAudioDevice();
}

void Game::run() const {
    while (!WindowShouldClose()) {
        gameState->update(GetFrameTime());

        BeginDrawing();
        ClearBackground(DARKGRAY);

        gameState->draw();

        EndDrawing();
    }
}

void Game::setGameState(GameState &gameState) {
    this->gameState = &gameState;
}
