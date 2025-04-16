#pragma once

#include <raylib.h>
#include <string>

#include "scene/Scene.h"
#include "../Player.h"

class TestScene final : public Scene {
    Camera2D camera;

    Texture2D tileSet{};
    int widthInTiles;
    int heightInTiles;
    Rectangle srcRect = { 16 * 2, 16 * 9, 16, 16 };

    std::string assets = ASSETS;

    Player *player;

    int tiles[6][5] = {
        {50, 48, 50, 50, 60},
        {50, 50, 48, 60, 50},
        {50, 48, 50, 50, 60},
        {50, 48, 50, 50, 60},
        {72, 50, 48, 60, 50},
        {50, 48, 50, 50, 60},
    };

public:
    TestScene();

    void update(float deltaTime) override;

    void draw() override;
};
