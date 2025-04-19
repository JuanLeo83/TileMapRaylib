#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>

// TODO: seleccionar múltiples tiles a la vez para pintarlos de una vez (ya veremos cómo lo hago)
// TODO: Modificar en caliente dimensiones del mapa
// TODO: Modificar en caliente dimensiones del tile
// TODO: Modificar en caliente el tileset
// TODO: Añadir capas de tiles
// TODO: Autotiling

TestScene::TestScene() {
    tileSet = LoadTexture((assets + "/tileset.png").c_str());
    tileSetZoneWidth = tileSet.width;
    tileSetWidthInTiles = 12;
    tileSetHeightInTiles = 10;
    tileWidth = 16;
    tileHeight = 16;
    tileMap = new TileMap(tileSet, tileSetWidthInTiles, tileSetHeightInTiles, tileWidth, tileHeight);
    tileMap->setPosition({static_cast<float>(tileSetZoneWidth), 0});
    tileMap->initEmptyTiles(worldWidth, worldHeight);
    tileMap->loadMap("../assets/savedMap.tm");

    initCamera(cameraMap);
    initCamera(cameraTileSet);
    initCamera(cameraTileSelected);

    mousePosition = GetMousePosition();
    worldPositionMap = GetScreenToWorld2D(GetMousePosition(), cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);

    selectedTile = 0;
}

void TestScene::update(const float deltaTime) {
    mousePosition = GetMousePosition();
    worldPositionMap = GetScreenToWorld2D(mousePosition, cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);

    // Scroll
    if (isMouseInsideTileSetZone()) {
        cameraTileSet.zoom += GetMouseWheelMoveV().y * deltaTime;
    } else if (isMouseInsideTileMapZone()) {
        cameraMap.zoom += GetMouseWheelMoveV().y * deltaTime;
    }

    // Camera move
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        if (isMouseInsideTileSetZone()) {
            cameraTileSet.target.x -= GetMouseDelta().x;
            cameraTileSet.target.y -= GetMouseDelta().y;
        } else if (isMouseInsideTileMapZone()) {
            cameraMap.target.x -= GetMouseDelta().x;
            cameraMap.target.y -= GetMouseDelta().y;
        }
    }

    // Save
    if (IsKeyDown(KEY_ENTER)) {
        saveMapToFile(*tileMap, "../assets/savedMap.tm");
    }

    // Draw and select
    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (isMouseInsideTileSetZone() &&
            mousePosition.y > 0 && mousePosition.y < static_cast<float>(GetScreenHeight())) {
            const float tileX = worldPositionTileSet.x / static_cast<float>(tileWidth);
            const float tileY = worldPositionTileSet.y / static_cast<float>(tileHeight);
            selectedTile = static_cast<int>(tileY * static_cast<float>(tileSetZoneWidth / tileWidth) + tileX);
            selectedTilePosition.x = tileX * static_cast<float>(tileWidth);
            selectedTilePosition.y = tileY * static_cast<float>(tileHeight);
        }

        if (isMouseInsideTileMap()) {
            const float tileX = (worldPositionMap.x - static_cast<float>(tileSetZoneWidth)) / static_cast<float>(tileWidth);
            const float tileY = worldPositionMap.y / static_cast<float>(tileHeight);
            if (IsKeyDown(KEY_BACKSPACE)) {
                tileMap->setTile(tileX, tileY, NO_TILE);
            } else {
                tileMap->setTile(tileX, tileY, selectedTile);
            }
        }
    }
}

void TestScene::draw() {
    BeginScissorMode(tileSetZoneWidth, 0, GetScreenWidth(), GetScreenHeight());
    BeginMode2D(cameraMap);
    tileMap->draw();

    for (int v = 0; v <= worldWidth; v++) {
        DrawLine(tileSetZoneWidth + v * tileWidth, 0, tileSetZoneWidth + v * tileWidth, worldHeight * tileHeight, GRAY);
    }
    for (int h = 0; h <= worldHeight; h++) {
        DrawLine(tileSetZoneWidth, h * tileHeight, tileSetZoneWidth + worldWidth * tileWidth, h * tileHeight,
                 LIGHTGRAY);
    }
    EndMode2D();
    EndScissorMode();

    BeginScissorMode(0, 0, tileSetZoneWidth, GetScreenHeight() / 2);
    DrawRectangle(0, 0, tileSetZoneWidth, GetScreenHeight(), BLACK);
    BeginMode2D(cameraTileSet);
    DrawTexture(tileSet, 0, 0, WHITE);
    for (int v = 0; v < tileSetZoneWidth; v += tileWidth) {
        DrawLine(v, 0, v, tileSet.height, WHITE);
    }
    for (int h = 0; h < tileSet.height; h += tileHeight) {
        DrawLine(0, h, tileSetZoneWidth, h, WHITE);
    }

    DrawRectangleLines(static_cast<int>(selectedTilePosition.x), static_cast<int>(selectedTilePosition.y), tileWidth,
                       tileHeight, LIME);
    EndMode2D();
    EndScissorMode();

    BeginScissorMode(0, GetScreenHeight() / 2, tileSetZoneWidth, GetScreenHeight() / 2);
    BeginMode2D(cameraTileSelected);
    DrawRectangle(0, 0, tileSetZoneWidth, GetScreenHeight(), BLACK);
    DrawTexturePro(tileSet,
                   (Rectangle){
                       static_cast<float>(selectedTile % (tileSetZoneWidth / tileWidth) * tileWidth),
                       static_cast<float>(selectedTile / (tileSetZoneWidth / tileHeight) * tileHeight),
                       static_cast<float>(tileWidth),
                       static_cast<float>(tileHeight)
                   },
                   {
                       static_cast<float>(tileSetZoneWidth / 2 - tileWidth),
                       static_cast<float>(GetScreenHeight() / 2 + 15),
                       32, 32
                   },
                   {0, 0}, 0,
                   WHITE);
    EndMode2D();
    EndScissorMode();

    DrawRectangle(tileSetZoneWidth - 1, 0, 3, GetScreenHeight(), WHITE);
    DrawRectangle(0, GetScreenHeight() / 2 - 1, tileSetZoneWidth, 3, WHITE);
}

void TestScene::initCamera(Camera2D &camera) {
    camera = {};
    camera.target.x = 0;
    camera.target.y = 0;
    camera.offset.x = 0;
    camera.offset.y = 0;
    camera.zoom = 1;
}

bool TestScene::isMouseInsideTileSetZone() const {
    return mousePosition.x > 0 && mousePosition.x < static_cast<float>(tileSetZoneWidth);
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
