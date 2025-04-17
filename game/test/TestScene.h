#pragma once

#include <raylib.h>
#include <string>
#include <tileMap/TileMap.h>

#include "scene/Scene.h"

class TestScene final : public Scene {
    Camera2D camera;

    Texture2D tileSet{};

    std::string assets = ASSETS;

    TileMap *tileMap;

    Vector2 mousePosition;
    Vector2 worldPosition;

    int worldWidth = 20;
    int worldHeight = 30;
    int selectedTile = -1;
    Vector2 selectedTilePosition = {0, 0};

public:
    TestScene();

    void update(float deltaTime) override;

    void draw() override;
};
