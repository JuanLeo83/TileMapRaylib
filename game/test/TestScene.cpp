#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>

TestScene::TestScene() {
    tileSet = LoadTexture((assets + "/tileset.png").c_str());
    tileMap = new TileMap(tileSet, 12, 10, 16, 16);
    tileMap->setPosition({static_cast<float>(tileSet.width), 0});
    tileMap->initEmptyTiles(worldWidth, worldHeight);
    // tileMap->loadMap("../assets/myMap.tm");

    camera = {};
    camera.target.x = 0;
    camera.target.y = 0;
    camera.offset.x = 0;
    camera.offset.y = 0;
    camera.zoom = 1;

    worldPosition = GetScreenToWorld2D(GetMousePosition(), camera);

    selectedTile = 0;
}

void TestScene::update(const float deltaTime) {
    camera.zoom += GetMouseWheelMoveV().y * deltaTime;
    if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        camera.target.x -= GetMouseDelta().x;
        camera.target.y -= GetMouseDelta().y;
    }

    if (IsKeyDown(KEY_ENTER)) {
        saveMapToFile(*tileMap, "../assets/savedMap.tm");
    }

    const Vector2 mousePosition = GetMousePosition();
    worldPosition = GetScreenToWorld2D(mousePosition, camera);

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        if (mousePosition.x > 0 && mousePosition.x < tileSet.width &&
            mousePosition.y > 0 && mousePosition.y < tileSet.height) {
            const int tileX = mousePosition.x / 16;
            const int tileY = mousePosition.y / 16;
            selectedTile = tileY * (tileSet.width / 16) + tileX;
            selectedTilePosition.x = tileX * 16;
            selectedTilePosition.y = tileY * 16;
        }

        if (worldPosition.x > tileMap->getPosition().x && worldPosition.x < tileMap->getPosition().x + worldWidth * 16 &&
            worldPosition.y > tileMap->getPosition().y && worldPosition.y < tileMap->getPosition().y + worldHeight * 16) {
            const int tileX = (worldPosition.x - tileSet.width) / 16;
            const int tileY = worldPosition.y / 16;
            tileMap->setTile(tileX, tileY, selectedTile);
        }
    }

    // std::cout << worldPosition.x << ", " << worldPosition.y << std::endl;
}

void TestScene::draw() {
    BeginMode2D(camera);

    tileMap->draw();

    for (int v = 0; v <= worldWidth; v++) {
        DrawLine(tileSet.width + v * 16, 0, tileSet.width + v * 16, worldHeight * 16, GRAY);
    }
    for (int h = 0; h <= worldHeight; h++) {
        DrawLine(tileSet.width, h * 16, tileSet.width + worldWidth * 16, h * 16, LIGHTGRAY);
    }

    EndMode2D();

    DrawRectangle(0, 0, tileSet.width, GetScreenHeight(), BLACK);
    DrawTexture(tileSet, 0, 0, WHITE);
    for (int v = 0; v < tileSet.width; v += 16) {
        DrawLine(v, 0, v, tileSet.height, WHITE);
    }
    for (int h = 0; h < tileSet.height; h += 16) {
        DrawLine(0, h, tileSet.width, h, WHITE);
    }

    DrawRectangleLines(selectedTilePosition.x, selectedTilePosition.y, 16, 16, LIME);

    DrawTextureRec(tileSet, (Rectangle){
                       static_cast<float>(selectedTile % (tileSet.width / 16) * 16),
                       static_cast<float>(selectedTile / (tileSet.width / 16) * 16), 16, 16
                   },
                   {static_cast<float>(tileSet.width / 2 - 8), static_cast<float>(tileSet.height + 30)},
                   WHITE);
}
