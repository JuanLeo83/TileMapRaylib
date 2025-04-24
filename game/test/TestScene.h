#pragma once

#include <raylib.h>
#include <string>
#include <tileMap/TileMap.h>

#include "scene/Scene.h"

class TestScene final : public Scene {
    bool unsavedChanges = false;

    std::string assets = ASSETS;
    const int NO_TILE = -1;
    Camera2D cameraTileSelected{};
    Camera2D cameraTileSet{};
    Camera2D cameraMap{};

    const int tileSetZoneWidth = 300;
    std::string tileSetPath = "";
    std::string tileSetName = "";
    Texture2D tileSet{};
    int tileSetWidthInTiles = 1;
    int tileSetHeightInTiles = 1;
    int tileWidth = 1;
    int tileHeight = 1;

    TileMap *tileMap;

    Vector2 mousePosition{};
    Vector2 worldPositionMap{};
    Vector2 worldPositionTileSet{};

    int worldWidth = 20;
    int worldHeight = 20;
    int selectedTile = 0;
    Vector2 selectedTilePosition = {0, 0};

    static void initCamera(Camera2D &camera);

    bool isMouseInsideTileSetZone() const;

    bool isMouseInsideTileSet() const;

    bool isMouseInsideTileMapZone() const;

    bool isMouseInsideTileMap() const;

    void saveMap(const std::string &filePath,  const std::string &fileName);

    void selectTile();

    void setTileData();

    void moveCamera();

    void drawMap() const;

    void drawTileSet() const;

    void drawSelectedTile() const;

    void drawGui();

    static void showSaveMapDialog();

    static void showLoadMapDialog();

    void loadMap(const std::string &filePath,  const std::string &fileName);

    void createNewMap();

    void confirmNewMap();

public:
    TestScene();

    void update(float deltaTime) override;

    void draw() override;
};
