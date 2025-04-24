#include "TestScene.h"

#include <raylib.h>
#include <tileMap/TileMapUtils.h>
#include "rlImGui.h"
#include "imgui.h"
#include "ImGuiFileDialog.h"

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
}

TestScene::~TestScene() {
    UnloadTexture(tileSet);
    delete tileMap;
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
        showSaveMapDialog();
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

    drawGui();
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

void TestScene::drawGui() {
    rlImGuiBegin();

    drawGuiSettings();
    drawGuiTileSet();
    drawGuiTileMap();

    rlImGuiEnd();
}

void TestScene::drawGuiSettings() {
    ImGui::SetNextWindowPos(ImVec2(GetScreenWidth() - 300, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, GetScreenHeight()), ImGuiCond_Always);

    if (ImGui::Begin("Config", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("New map")) {
            if (unsavedChanges) {
                ImGui::OpenPopup("New map: Warning");
            } else {
                createNewMap();
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("TileSet: %s", tileSetName.c_str());
        ImGui::Separator();
        if (ImGui::Button("Select TileSet")) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".png,.jpeg,.jpg,.*", config);
        }

        if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                tileSetPath = ImGuiFileDialog::Instance()->GetFilePathName();
                tileSetName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                tileMap->setTileSetPath(tileSetPath);
            }

            ImGuiFileDialog::Instance()->Close();
        }

        ImGui::InputInt("Map width", &worldWidth);
        ImGui::InputInt("Map height", &worldHeight);

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        if (unsavedChanges) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); // Cambiar color a rojo
            ImGui::Text("TileMap: %s*", tileMap->getTileMapName().c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Unsaved changes");
            }
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("TileMap: %s", tileMap->getTileMapName().c_str());
        }
        ImGui::Separator();

        ImGui::InputInt("Tile width", &tileWidth);
        ImGui::InputInt("Tile height", &tileHeight);

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Load map")) {
            showLoadMapDialog();
        }

        ImGui::SameLine();
        if (ImGui::Button("Save map")) {
            showSaveMapDialog();
        }

        if (ImGuiFileDialog::Instance()->Display("LoadMapDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                std::string loadPath = ImGuiFileDialog::Instance()->GetFilePathName();
                std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                loadMap(loadPath, fileName);
            }

            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display("SaveMapDlgKey")) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                const std::string savePath = ImGuiFileDialog::Instance()->GetFilePathName();
                const std::string fileName = ImGuiFileDialog::Instance()->GetCurrentFileName();
                saveMap(savePath, fileName);
            }

            ImGuiFileDialog::Instance()->Close();
        }

        confirmNewMap();
    }
    ImGui::End();
}

void TestScene::drawGuiTileSet() {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(299, 55), ImGuiCond_Always);
    if (ImGui::Begin("TileSet Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("Reset position")) {
            cameraTileSet.target = {0, 0};
            cameraTileSet.zoom = 1;
        }
    }
    ImGui::End();
}

void TestScene::drawGuiTileMap() {
    ImGui::SetNextWindowPos(ImVec2(302, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(GetScreenWidth() - 602, 200), ImGuiCond_Always);

    if (ImGui::Begin("TileMap Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("Reset position")) {
            cameraMap.target = {0, 0};
            cameraMap.zoom = 1;
        }

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::BeginPopupModal("Remove layer confirmation", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("Are you sure you want to delete the selected layer?\nThis action cannot be undone.");
            ImGui::Separator();
            ImGui::Spacing();

            constexpr float buttonWidth = 60.0f;
            const float spacing = ImGui::GetStyle().ItemSpacing.x;
            const float totalWidth = 2 * buttonWidth + spacing;
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalWidth - ImGui::GetStyle().WindowPadding.x);

            if (ImGui::Button("Yes", ImVec2(60, 0))) {
                tileMap->removeLayer(activeLayer);
                activeLayer = std::max(0, activeLayer - 1);
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No", ImVec2(60, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }

        ImGui::Text("Active layer:");
        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("##activeLayer", std::to_string(activeLayer).c_str())) {
            for (int i = 0; i < tileMap->getLayers().size(); ++i) {
                const bool isSelected = activeLayer == i;
                if (ImGui::Selectable(std::to_string(i).c_str(), isSelected)) {
                    activeLayer = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::Button("Add layer")) {
            tileMap->addLayer();
        }
        ImGui::SameLine();
        if (ImGui::Button("Remove layer")) {
            ImGui::OpenPopup("Remove layer confirmation");
        }

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Move content from selected layer to:");
        ImGui::SetNextItemWidth(100);
        if (ImGui::BeginCombo("##targetLayer", std::to_string(targetLayer).c_str())) {
            for (int i = 0; i < tileMap->getLayers().size(); ++i) {
                const bool isSelected = (targetLayer == i);
                if (ImGui::Selectable(std::to_string(i).c_str(), isSelected)) {
                    targetLayer = i;
                }
                if (isSelected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if (ImGui::Button("Move content")) {
            if (activeLayer != targetLayer) {
                auto &layers = tileMap->getLayers();
                layers[targetLayer] = layers[activeLayer];
                layers[activeLayer] = std::vector(worldHeight, std::vector<int>(worldWidth, NO_TILE));
            } else {
                ImGui::OpenPopup("Error");
            }
        }

        if (ImGui::BeginPopupModal("Error", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text("You cannot move the content to the same layer.");
            ImGui::Separator();
            ImGui::Spacing();

            constexpr float buttonWidth = 60.0f;
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - buttonWidth - ImGui::GetStyle().WindowPadding.x);

            if (ImGui::Button("Close", ImVec2(buttonWidth, 0))) {
                ImGui::CloseCurrentPopup();
            }

            ImGui::EndPopup();
        }
    }
    ImGui::End();
}

void TestScene::showSaveMapDialog() {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("SaveMapDlgKey", "Save Map As", ".tm", config);
}

void TestScene::showLoadMapDialog() {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog("LoadMapDlgKey", "Load Map", ".tm", config);
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

void TestScene::confirmNewMap() {
    if (unsavedChanges) {
        if (ImGui::BeginPopupModal("New map: Warning", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(
                "There are unsaved changes. Do you want to continue?\nThis action will discard the current changes.");
            ImGui::Separator();
            ImGui::Spacing();

            constexpr float buttonWidth = 60.0f;
            const float spacing = ImGui::GetStyle().ItemSpacing.x;
            const float totalWidth = 2 * buttonWidth + spacing;
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalWidth - ImGui::GetStyle().WindowPadding.x);

            if (ImGui::Button("Yes", ImVec2(buttonWidth, 0))) {
                createNewMap();
                ImGui::CloseCurrentPopup();
            }
            ImGui::SameLine();
            if (ImGui::Button("No", ImVec2(buttonWidth, 0))) {
                ImGui::CloseCurrentPopup();
            }
            ImGui::EndPopup();
        }
    }
}
