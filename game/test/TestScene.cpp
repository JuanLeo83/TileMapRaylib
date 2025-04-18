#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>

// TODO: seleccionar múltiples tiles a la vez para pintarlos de una vez
// TODO: Borrar tile del mapa
// TODO: Modificar en caliente dimensiones del mapa
// TODO: Modificar en caliente dimensiones del tile
// TODO: Añadir capas de tiles
// TODO: Autotiling

TestScene::TestScene() {
    tileSet = LoadTexture((assets + "/tileset.png").c_str());
    tileMap = new TileMap(tileSet, 12, 10, 16, 16);
    tileMap->setPosition({static_cast<float>(tileSet.width), 0});
    tileMap->initEmptyTiles(worldWidth, worldHeight);
    // tileMap->loadMap("../assets/savedMap.tm");

    cameraMap = {};
    cameraMap.target.x = 0;
    cameraMap.target.y = 0;
    cameraMap.offset.x = 0;
    cameraMap.offset.y = 0;
    cameraMap.zoom = 1;

    cameraTileSet = {};
    cameraTileSet.target.x = 0;
    cameraTileSet.target.y = 0;
    cameraTileSet.offset.x = 0;
    cameraTileSet.offset.y = 0;
    cameraTileSet.zoom = 1;

    cameraTileSelected = {};
    cameraTileSelected.target.x = 0;
    cameraTileSelected.target.y = 0;
    cameraTileSelected.offset.x = 0;
    cameraTileSelected.offset.y = 0;
    cameraTileSelected.zoom = 1;

    worldPositionMap = GetScreenToWorld2D(GetMousePosition(), cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);

    selectedTile = 0;
}

void TestScene::update(const float deltaTime) {
    const Vector2 mousePosition = GetMousePosition();
    worldPositionMap = GetScreenToWorld2D(mousePosition, cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);

    // Scroll
    if (mousePosition.x > 0 && mousePosition.x < tileSet.width) {
        cameraTileSet.zoom += GetMouseWheelMoveV().y * deltaTime;
    } else if (mousePosition.x > tileSet.width && mousePosition.x < GetScreenWidth()) {
        cameraMap.zoom += GetMouseWheelMoveV().y * deltaTime;
    }

    // Camera move
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        if (mousePosition.x > 0 && mousePosition.x < tileSet.width) {
            cameraTileSet.target.x -= GetMouseDelta().x;
            cameraTileSet.target.y -= GetMouseDelta().y;
        } else if (mousePosition.x > tileSet.width && mousePosition.x < GetScreenWidth()) {
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
        if (mousePosition.x > 0 && mousePosition.x < tileSet.width &&
            mousePosition.y > 0 && mousePosition.y < GetScreenHeight()) {
            const int tileX = worldPositionTileSet.x / 16;
            const int tileY = worldPositionTileSet.y / 16;
            selectedTile = tileY * (tileSet.width / 16) + tileX;
            selectedTilePosition.x = tileX * 16;
            selectedTilePosition.y = tileY * 16;
        }

        if (worldPositionMap.x > tileMap->getPosition().x && worldPositionMap.x < tileMap->getPosition().x + worldWidth
            * 16
            &&
            worldPositionMap.y > tileMap->getPosition().y && worldPositionMap.y < tileMap->getPosition().y + worldHeight
            *
            16) {
            const int tileX = (worldPositionMap.x - tileSet.width) / 16;
            const int tileY = worldPositionMap.y / 16;
            tileMap->setTile(tileX, tileY, selectedTile);
        }
    }
}

void TestScene::draw() {
    BeginScissorMode(tileSet.width, 0, GetScreenWidth(), GetScreenHeight());
        BeginMode2D(cameraMap);
            tileMap->draw();

            for (int v = 0; v <= worldWidth; v++) {
                DrawLine(tileSet.width + v * 16, 0, tileSet.width + v * 16, worldHeight * 16, GRAY);
            }
            for (int h = 0; h <= worldHeight; h++) {
                DrawLine(tileSet.width, h * 16, tileSet.width + worldWidth * 16, h * 16, LIGHTGRAY);
            }
        EndMode2D();
    EndScissorMode();

    BeginScissorMode(0, 0, tileSet.width, GetScreenHeight() / 2);
        DrawRectangle(0, 0, tileSet.width, GetScreenHeight(), BLACK);
        BeginMode2D(cameraTileSet);
            DrawTexture(tileSet, 0, 0, WHITE);
            for (int v = 0; v < tileSet.width; v += 16) {
                DrawLine(v, 0, v, tileSet.height, WHITE);
            }
            for (int h = 0; h < tileSet.height; h += 16) {
                DrawLine(0, h, tileSet.width, h, WHITE);
            }

            DrawRectangleLines(selectedTilePosition.x, selectedTilePosition.y, 16, 16, LIME);
        EndMode2D();
    EndScissorMode();

    BeginScissorMode(0, GetScreenHeight() / 2, tileSet.width, GetScreenHeight() / 2);
        BeginMode2D(cameraTileSelected);
            DrawRectangle(0, 0, tileSet.width, GetScreenHeight(), BLACK);
            DrawTexturePro(tileSet,
                (Rectangle){
                       static_cast<float>(selectedTile % (tileSet.width / 16) * 16),
                       static_cast<float>(selectedTile / (tileSet.width / 16) * 16), 16, 16
                   },
                   {
                       static_cast<float>(tileSet.width / 2 - 16),
                       static_cast<float>(GetScreenHeight() / 2 + 15),
                           32, 32
                   },
                   {0, 0}, 0,
                   WHITE);
        EndMode2D();
    EndScissorMode();

    DrawRectangle(tileSet.width - 1, 0, 3, GetScreenHeight(), WHITE);
    DrawRectangle(0, GetScreenHeight() / 2 - 1, tileSet.width, 3, WHITE);
}
