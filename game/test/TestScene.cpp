#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>

TestScene::TestScene() {
    tileSet = LoadTexture((assets + "/tileset.png").c_str());
    tileMap = new TileMap(tileSet, 12, 10, 16, 16);

    player = new Player();

    camera = {};
    camera.target.x = 0;
    camera.target.y = 0;
    camera.offset.x = 0;
    camera.offset.y = 0;
    camera.zoom = 1;
}

void TestScene::update(float deltaTime) {
    if (IsKeyDown(KEY_W)) {
        camera.target.y -= 100 * deltaTime;
    }
    if (IsKeyDown(KEY_S)) {
        camera.target.y += 100 * deltaTime;
    }
    if (IsKeyDown(KEY_A)) {
        camera.target.x -= 100 * deltaTime;
    }
    if (IsKeyDown(KEY_D)) {
        camera.target.x += 100 * deltaTime;
    }
    if (IsKeyDown(KEY_Q)) {
        camera.zoom -= 0.5f * deltaTime;
    }
    if (IsKeyDown(KEY_E)) {
        camera.zoom += 0.5f * deltaTime;
    }
    if (IsKeyDown(KEY_ENTER)) {
        saveMapToFile(*tileMap, "../assets/savedMap.tm");
    }

    // player->update(deltaTime);
}

void TestScene::draw() {
    BeginMode2D(camera);

    tileMap->draw();

    // player->draw();

    EndMode2D();
}
