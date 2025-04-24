#include <raylib.h>
#include "Game.h"

#include "imgui.h"
#include "rlImGui.h"

Game::Game(const int width, const int height, const char *title) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(width, height, title);
    SetTargetFPS(60);

    ImGuiIO &io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigWindowsMoveFromTitleBarOnly = true;

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

        DrawFPS(10, GetScreenHeight() - 20);

        EndDrawing();
    }
}

void Game::setGameState(GameState &gameState) {
    this->gameState = &gameState;
}
