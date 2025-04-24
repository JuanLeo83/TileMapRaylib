#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>
#include "rlImGui.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

// TODO: seleccionar múltiples tiles a la vez para pintarlos de una vez (ya veremos cómo lo hago)
// TODO: Añadir capas de tiles
// TODO: cargar mapa
// TODO: crear nuevo mapa
// TODO: Autotiling

TestScene::TestScene() {
    tileSet = LoadTexture(tileSetPath.c_str());
    tileMap = new TileMap(tileSetPath, tileSet, tileSetWidthInTiles, tileSetHeightInTiles, tileWidth, tileHeight);
    tileMap->setPosition({static_cast<float>(tileSetZoneWidth), 0});
    tileMap->initEmptyTiles(worldWidth, worldHeight);

    tileMap->loadMap("../assets/savedMap.tm", worldWidth, worldHeight, tileWidth, tileHeight);

    initCamera(cameraMap);
    initCamera(cameraTileSet);
    initCamera(cameraTileSelected);

    mousePosition = GetMousePosition();
    worldPositionMap = GetScreenToWorld2D(GetMousePosition(), cameraMap);
    worldPositionTileSet = GetScreenToWorld2D(GetMousePosition(), cameraTileSet);
}

void TestScene::update(const float deltaTime) {
    if (ImGui::GetIO().WantCaptureMouse) return;

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
        save();
    }

    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && isMouseInsideTileSet()) {
        selectTile();
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && isMouseInsideTileMap()) {
        setTileData();
    }

}

void TestScene::draw() {
    // Map
    drawMap();

    // TileSet
    drawTileSet();

    // Selected tile
    drawSelectedTile();

    DrawRectangle(tileSetZoneWidth - 1, 0, 3, GetScreenHeight(), WHITE);
    DrawRectangle(0, GetScreenHeight() / 2 - 1, tileSetZoneWidth, 3, WHITE);

    drawGui();
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
    return mousePosition.x > 0 && mousePosition.x < static_cast<float>(tileSetZoneWidth) &&
           mousePosition.y > 0 && mousePosition.y < static_cast<float>(GetScreenHeight()) / 2;
}

bool TestScene::isMouseInsideTileSet() const {
    return worldPositionTileSet.x > 0 && worldPositionTileSet.x < tileSet.width && mousePosition.x < tileSetZoneWidth &&
           worldPositionTileSet.y > 0 && worldPositionTileSet.y < tileSet.height && mousePosition.y < GetScreenHeight() / 2;
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

void TestScene::save() const {
    saveMapToFile(*tileMap, "../assets/savedMap.tm");
}

void TestScene::selectTile() {
    const int tileX = worldPositionTileSet.x / tileWidth;
    const int tileY = worldPositionTileSet.y / tileHeight;
    selectedTile = tileY * tileSet.width / tileWidth + tileX;
    selectedTilePosition.x = tileX * tileWidth;
    selectedTilePosition.y = tileY * tileHeight;
}

void TestScene::setTileData() const {
    const float tileX = (worldPositionMap.x - static_cast<float>(tileSetZoneWidth)) / static_cast<float>(tileWidth);
    const float tileY = worldPositionMap.y / static_cast<float>(tileHeight);

    if (IsKeyDown(KEY_BACKSPACE)) {
        tileMap->setTile(tileX, tileY, NO_TILE);
    } else {
        tileMap->setTile(tileX, tileY, selectedTile);
    }
}

void TestScene::moveCamera() {
    if (isMouseInsideTileSetZone()) {
        cameraTileSet.target.x -= GetMouseDelta().x;
        cameraTileSet.target.y -= GetMouseDelta().y;
    } else if (isMouseInsideTileMapZone()) {
        cameraMap.target.x -= GetMouseDelta().x;
        cameraMap.target.y -= GetMouseDelta().y;
    }
}

void TestScene::drawMap() const {
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
}

void TestScene::drawTileSet() const {
    BeginScissorMode(0, 0, tileSetZoneWidth, GetScreenHeight() / 2);
    DrawRectangle(0, 0, tileSetZoneWidth, GetScreenHeight(), BLACK);
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
    BeginScissorMode(0, GetScreenHeight() / 2, tileSetZoneWidth, GetScreenHeight() / 2);
    BeginMode2D(cameraTileSelected);
    DrawRectangle(0, 0, tileSetZoneWidth, GetScreenHeight(), BLACK);
    DrawTexturePro(tileSet,
                   (Rectangle){
                       selectedTilePosition.x,
                       selectedTilePosition.y,
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
}

void TestScene::drawGui() {
    rlImGuiBegin();

    ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() - 300, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, GetScreenHeight()), ImGuiCond_Always);

    if (ImGui::Begin("Config", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("Select TileSet")) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png,.jpeg,.jpg,.*", config);
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                tileSetPath = ImGuiFileDialog::Instance()->GetFilePathName();
                tileMap->setTileSetPath(tileSetPath);
            }

            ImGuiFileDialog::Instance()->Close();
        }
        ImGui::SameLine();
        ImGui::Text(tileSetPath.c_str());

        ImGui::InputInt("Map width", &worldWidth);
        ImGui::InputInt("Map height", &worldHeight);

        ImGui::InputInt("Tile width", &tileWidth);
        ImGui::InputInt("Tile height", &tileHeight);

        if (ImGui::Button("Save")) {
            save();
        }
    }
    ImGui::End();
    rlImGuiEnd();
}
