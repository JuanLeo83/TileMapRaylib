#pragma once

#include <raylib.h>
#include <string>
#include <tileMap/TileMap.h>

#include "AutoTiler.h"
#include "GuiManager.h"
#include "scene/Scene.h"

class EditorScene final : public Scene {
    bool unsavedChanges = false;

    std::string assets = ASSETS;
    Camera2D cameraTileSelected{};
    Camera2D cameraTileSet{};
    Camera2D cameraMap{};

    const int tileSetZoneWidth = 300;
    std::string tileSetPath;
    std::string tileSetName;
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

    int activeLayer = 0;
    int targetLayer = 0;
    int amountOfRandomTiles = 10;

    GuiManager *guiManager;
    AutoTiler *autoTiler;

    bool autoTiling = false;

    static void initCamera(Camera2D &camera);

    bool isMouseInsideTileSetZone() const;

    bool isMouseInsideTileSet() const;

    bool isMouseInsideTileMapZone() const;

    bool isMouseInsideTileMap() const;

    void selectTile();

    void setTileData();

    void moveCamera();

    void drawMap() const;

    void drawTileSet() const;

    void drawSelectedTile() const;

public:
    const int NO_TILE = -1;

    EditorScene();

    ~EditorScene() override;

    [[nodiscard]] TileMap *getTileMap() const {
        return tileMap;
    }

    void update(float deltaTime) override;

    void draw() override;

    void resetCameraTileSet() {
        cameraTileSet.target = {0, 0};
        cameraTileSet.zoom = 1;
    }

    void resetCameraMap() {
        cameraMap.target = {0, 0};
        cameraMap.zoom = 1;
    }

    void paintRandomTiles();

    int &getActiveLayer() {
        return activeLayer;
    }

    void setActiveLayer(const int layer){
        activeLayer = layer;
    }

    int &getTargetLayer() {
        return targetLayer;
    }

    void setTargetLayer(const int layer){
        targetLayer = layer;
    }

    int &getWorldWidth() {
        return worldWidth;
    }

    void setWorldWidth(const int width) {
        worldWidth = width;
    }

    int &getWorldHeight() {
        return worldHeight;
    }

    void setWorldHeight(const int height) {
        worldHeight = height;
    }

    int &getAmountOfRandomTiles() {
        return amountOfRandomTiles;
    }

    int &getTileWidth() {
        return tileWidth;
    }

    int &getTileHeight() {
        return tileHeight;
    }

    std::string &getTileSetName() {
        return tileSetName;
    }

    void setTileSetName(const std::string &name) {
        tileSetName = name;
    }

    void setTileSetPath(const std::string &path);

    void createNewMap();

    void loadMap(const std::string &filePath, const std::string &fileName);

    void saveMap(const std::string &filePath, const std::string &fileName);

    bool &getUnsavedChanges() {
        return unsavedChanges;
    }

    bool &getAutotiling() {
        return autoTiling;
    }
};
