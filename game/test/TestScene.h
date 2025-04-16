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
    Rectangle tileRect = { 16 * 2, 16 * 9, 16, 16 };
    Vector2 tilePosition = { 0, 0 };

    std::string assets = ASSETS;

    Player *player;

public:
    TestScene();

    void update(float deltaTime) override;

    void draw() const override;
};
