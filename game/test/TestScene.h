#pragma once

#include <raylib.h>
#include <string>
#include <tileMap/TileMap.h>

#include "scene/Scene.h"

class TestScene final : public Scene {
    const int NO_TILE = -1;
    Camera2D cameraTileSelected{};
    Camera2D cameraTileSet{};
    Camera2D cameraMap{};

    int tileSetZoneWidth;
    Texture2D tileSet{};
    int tileSetWidthInTiles;
    int tileSetHeightInTiles;
    int tileWidth;
    int tileHeight;

    std::string assets = ASSETS;

    TileMap *tileMap;

    Vector2 mousePosition{};
    Vector2 worldPositionMap{};
    Vector2 worldPositionTileSet{};

    int worldWidth = 0;
    int worldHeight = 0;
    int selectedTile = NO_TILE;
    Vector2 selectedTilePosition = {0, 0};

    static void initCamera(Camera2D &camera);

    bool isMouseInsideTileSetZone() const;

    bool isMouseInsideTileSet() const;

    bool isMouseInsideTileMapZone() const;

    bool isMouseInsideTileMap() const;

public:
    TestScene();

    void update(float deltaTime) override;

    void draw() override;
};
