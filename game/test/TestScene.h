#pragma once

#include <raylib.h>
#include <string>
#include <tileMap/TileMap.h>

#include "scene/Scene.h"
#include "../Player.h"

class TestScene final : public Scene {
    Camera2D camera;

    Texture2D tileSet{};

    std::string assets = ASSETS;

    Player *player;
    TileMap *tileMap;

public:
    TestScene();

    void update(float deltaTime) override;

    void draw() override;
};
