#include "TestScene.h"

#include <iostream>
#include <cmath>
#include <raylib.h>

TestScene::TestScene() {
    tileSet = LoadTexture((assets + "/tileset.png").c_str());
    widthInTiles = std::ceil(GetScreenWidth() / 16.0);
    heightInTiles = std::ceil(GetScreenHeight() / 16.0);

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
        camera.zoom -= 0.5f * deltaTime;
    }
    if (IsKeyDown(KEY_E)) {
        camera.zoom += 0.5f * deltaTime;
    }

    // player->update(deltaTime);
}

void TestScene::draw() {
    BeginMode2D(camera);

    for (int row = 0; row < 6; ++row) {
        for (int col = 0; col < 5; ++col) {
            const int tileIndex = tiles[row][col];
            const int tileRow = tileIndex / 12;
            srcRect.x = static_cast<float>((tileIndex - tileRow * 12) * 16);
            srcRect.y = static_cast<float>(tileRow * 16);

            DrawTextureRec(
                tileSet,
                srcRect,
                {static_cast<float>(col * 16), static_cast<float>(row * 16)},
                WHITE
            );
        }
    }

    // player->draw();

    EndMode2D();
}
