#include "TestScene.h"

#include <iostream>
#include <cmath>
#include <raylib.h>

TestScene::TestScene() {
    tileSet = LoadTexture((assets + "/tileset.png").c_str());
    widthInTiles = std::ceil(GetScreenWidth() / 16.0);
    heightInTiles = std::ceil(GetScreenHeight() / 16.0);
    std::cout << heightInTiles << std::endl;

    player = new Player();

    camera = {};
    camera.target.x = GetScreenWidth() / 2;
    camera.target.y = GetScreenHeight() / 2;
    camera.offset.x = GetScreenWidth() / 2;
    camera.offset.y = GetScreenWidth() / 2;
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
        camera.zoom -= 0.1f * deltaTime;
    }
    if (IsKeyDown(KEY_E)) {
        camera.zoom += 0.1f * deltaTime;
    }

    player->update(deltaTime);
}

void TestScene::draw() const {
    BeginMode2D(camera);

    for (int i = 0; i < widthInTiles; ++i) {
        for (int j = 0; j < heightInTiles; ++j) {
            DrawTextureRec(
                tileSet,
                tileRect,
                {static_cast<float>(i * 16), static_cast<float>(j * 16)},
                WHITE
            );
        }
    }

    player->draw();

    EndMode2D();
}
