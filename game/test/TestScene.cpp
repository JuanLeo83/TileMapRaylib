#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>
#include "GuiManager.h"

// TODO: seleccionar múltiples tiles a la vez para pintarlos de una vez (ya veremos cómo lo hago)
// TODO: Autotiling

TestScene::TestScene() {
    tileSet = LoadTexture(tileSetPath.c_str());
    tileMap = new TileMap(tileSetPath, tileSet, tileSetWidthInTiles, tileSetHeightInTiles, tileWidth, tileHeight);
    tileMap->setPosition({static_cast<float>(tileSetZoneWidth), 0});
    tileMap->initEmptyTiles(worldWidth, worldHeight, 1);

    initCamera(cameraMap);
    initCamera(cameraTileSet);
    initCamera(cameraTileSelected);

    mousePosition = GetMousePosition();
    worldPositionMap = GetScreenToWorld2D(GetMousePosition(), cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);

    guiManager = new GuiManager(this);
}

TestScene::~TestScene() {
    UnloadTexture(tileSet);
    delete tileMap;
}

void TestScene::update(const float deltaTime) {
    if (GuiManager::hasMouseFocus()) return;

    mousePosition = GetMousePosition();
    worldPositionMap = GetScreenToWorld2D(mousePosition, cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);
    tileMap->updateDimens(worldWidth, worldHeight, tileWidth, tileHeight);

    if (isMouseInsideTileSetZone()) {
        cameraTileSet.zoom += GetMouseWheelMoveV().y * deltaTime;
    } else if (isMouseInsideTileMapZone()) {
        cameraMap.zoom += GetMouseWheelMoveV().y * deltaTime;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT) || IsMouseButtonDown(MOUSE_BUTTON_MIDDLE)) {
        moveCamera();
    }

    if ((IsKeyDown(KEY_LEFT_CONTROL) || IsKeyDown(KEY_RIGHT_CONTROL)) && IsKeyPressed(KEY_S)) {
        GuiManager::showSaveMapDialog();
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMouseInsideTileSet()) {
        selectTile();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isMouseInsideTileMap()) {
        setTileData();
    }
}

void TestScene::draw() {
    drawMap();
    drawTileSet();
    drawSelectedTile();

    DrawRectangle(tileSetZoneWidth - 1, 0, 3, GetScreenHeight(), WHITE);
    DrawRectangle(0, GetScreenHeight() * 0.8f - 1, tileSetZoneWidth, 3, WHITE);

    guiManager->drawGui();
}

void TestScene::initCamera(Camera2D &camera) {
    camera = {};
    camera.zoom = 1;
}

bool TestScene::isMouseInsideTileSetZone() const {
    return mousePosition.x > 0 && mousePosition.x < static_cast<float>(tileSetZoneWidth) &&
           mousePosition.y > 0 && mousePosition.y < static_cast<float>(GetScreenHeight()) * 0.8f;
}

bool TestScene::isMouseInsideTileSet() const {
    return worldPositionTileSet.x > 0 && worldPositionTileSet.x < tileSet.width && mousePosition.x < tileSetZoneWidth &&
           worldPositionTileSet.y > 0 && worldPositionTileSet.y < tileSet.height && mousePosition.y < GetScreenHeight()
           * 0.8f;
}

bool TestScene::isMouseInsideTileMapZone() const {
    return mousePosition.x > static_cast<float>(tileSetZoneWidth) && mousePosition.x < static_cast<float>(
               GetScreenWidth());
}

bool TestScene::isMouseInsideTileMap() const {
    return worldPositionMap.x > tileMap->getPosition().x && worldPositionMap.x < tileMap->getPosition().x +
           static_cast<float>(worldWidth * tileWidth)
           &&
           worldPositionMap.y > tileMap->getPosition().y && worldPositionMap.y < tileMap->getPosition().y +
           static_cast<float>(worldHeight * tileHeight);
}

void TestScene::saveMap(const std::string &filePath, const std::string &fileName) {
    saveMapToFile(*tileMap, filePath);
    tileMap->setTileMapName(fileName);
    unsavedChanges = false;
}

void TestScene::selectTile() {
    const int tileX = worldPositionTileSet.x / tileWidth;
    const int tileY = worldPositionTileSet.y / tileHeight;
    selectedTile = tileY * tileSet.width / tileWidth + tileX;
    selectedTilePosition.x = tileX * tileWidth;
    selectedTilePosition.y = tileY * tileHeight;
}

void TestScene::setTileData() {
    const float tileX = (worldPositionMap.x - static_cast<float>(tileSetZoneWidth)) / static_cast<float>(tileWidth);
    const float tileY = worldPositionMap.y / static_cast<float>(tileHeight);

    if (IsKeyDown(KEY_BACKSPACE)) {
        tileMap->setTile(tileX, tileY, NO_TILE, activeLayer);
    } else if (IsKeyDown(KEY_LEFT_SHIFT)) {
        tileMap->floodFill(tileX, tileY, activeLayer, selectedTile);
    } else {
        tileMap->setTile(tileX, tileY, selectedTile, activeLayer);
    }

    unsavedChanges = true;
}

void TestScene::moveCamera() {
    Camera2D *camera = nullptr;
    if (isMouseInsideTileSetZone()) {
        camera = &cameraTileSet;
    } else if (isMouseInsideTileMapZone()) {
        camera = &cameraMap;
    }

    if (camera) {
        camera->target.x -= GetMouseDelta().x / camera->zoom;
        camera->target.y -= GetMouseDelta().y / camera->zoom;
    }
}

void TestScene::drawMap() const {
    BeginScissorMode(tileSetZoneWidth, 0, GetScreenWidth(), GetScreenHeight());
    BeginMode2D(cameraMap);
    tileMap->draw(activeLayer);

    for (int v = 0; v <= worldWidth; v++) {
        DrawLine(tileSetZoneWidth + v * tileWidth, 0, tileSetZoneWidth + v * tileWidth, worldHeight * tileHeight, GRAY);
    }
    for (int h = 0; h <= worldHeight; h++) {
        DrawLine(tileSetZoneWidth, h * tileHeight, tileSetZoneWidth + worldWidth * tileWidth, h * tileHeight,
                 LIGHTGRAY);
    }
    EndMode2D();
    EndScissorMode();
}

void TestScene::drawTileSet() const {
    BeginScissorMode(0, 0, tileSetZoneWidth, GetScreenHeight() * 0.8f);
    DrawRectangle(0, 0, tileSetZoneWidth, GetScreenHeight() * 0.8f, BLACK);
    BeginMode2D(cameraTileSet);
    DrawTexture(tileSet, 0, 0, WHITE);
    for (int v = 0; v <= tileSet.width; v += tileWidth) {
        DrawLine(v, 0, v, tileSet.height, WHITE);
    }
    for (int h = 0; h <= tileSet.height; h += tileHeight) {
        DrawLine(0, h, tileSet.width, h, WHITE);
    }

    DrawRectangleLines(static_cast<int>(selectedTilePosition.x), static_cast<int>(selectedTilePosition.y), tileWidth,
                       tileHeight, LIME);
    EndMode2D();
    EndScissorMode();
}

void TestScene::drawSelectedTile() const {
    BeginScissorMode(0, GetScreenHeight() * 0.8f, tileSetZoneWidth, GetScreenHeight() * 0.2f);
    BeginMode2D(cameraTileSelected);
    DrawRectangle(0, GetScreenHeight() * 0.8f, tileSetZoneWidth, GetScreenHeight() * 0.2f, BLACK);
    DrawTexturePro(tileSet,
                   (Rectangle){
                       selectedTilePosition.x,
                       selectedTilePosition.y,
                       static_cast<float>(tileWidth),
                       static_cast<float>(tileHeight)
                   },
                   {
                       static_cast<float>(tileSetZoneWidth / 2 - tileWidth),
                       static_cast<float>(GetScreenHeight() * 0.8 + 15),
                       96, 96
                   },
                   {0, 0}, 0,
                   WHITE);
    EndMode2D();
    EndScissorMode();
}

void TestScene::loadMap(const std::string &filePath, const std::string &fileName) {
    int layerCount = 0;
    tileMap->loadMap(filePath, fileName, worldWidth, worldHeight, tileWidth, tileHeight, layerCount);
    tileSetName = tileSetPath.substr(tileSetPath.find_last_of("/\\") + 1);
}

void TestScene::createNewMap() {
    worldWidth = 20;
    worldHeight = 20;
    tileWidth = 16;
    tileHeight = 16;
    tileMap->initEmptyTiles(worldWidth, worldHeight, 1);
    tileSetPath = "";
    tileSetName = "";
    unsavedChanges = false;
    tileMap->setTileMapName("");
}

void TestScene::paintRandomTiles() {
    std::srand(static_cast<unsigned>(std::time(nullptr)));

    for (int i = 0; i < amountOfRandomTiles; ++i) {
        const int randomX = std::rand() % worldWidth;
        const int randomY = std::rand() % worldHeight;

        tileMap->setTile(randomX, randomY, selectedTile, activeLayer);
    }

    unsavedChanges = true;
}

void TestScene::setTileSetPath(const std::string &path) {
    tileSetPath = path;
    tileMap->setTileSetPath(path);
}
