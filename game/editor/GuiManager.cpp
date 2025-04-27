#include "GuiManager.h"

#include "imgui.h"
#include "ImGuiFileDialog.h"
#include "rlImGui.h"
#include "EditorScene.h"

GuiManager::GuiManager(EditorScene *testScene) : editor(testScene) {
}

bool GuiManager::hasMouseFocus() {
    return ImGui::GetIO().WantCaptureMouse;
}

void GuiManager::drawGui() const {
    rlImGuiBegin();

    drawGuiSettings();
    drawGuiTileSet();
    drawGuiTileMap();

    rlImGuiEnd();
}

void GuiManager::drawGuiTileSet() const {
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(299, 90), ImGuiCond_Always);
    if (ImGui::Begin("TileSet Controls", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("Reset position")) {
            editor->resetCameraTileSet();
        }

        sectionSeparator();

        ImGui::Checkbox("Autotiling", &editor->getAutotiling());
    }
    ImGui::End();
}

void GuiManager::drawGuiTileMap() const {
    ImGui::SetNextWindowPos(ImVec2(302, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(static_cast<float>(GetScreenWidth()) - 602, 230), ImGuiCond_Appearing);

    if (ImGui::Begin("TileMap Controls", nullptr, ImGuiWindowFlags_NoMove)) {
        if (ImGui::Button("Reset position")) {
            editor->resetCameraMap();
        }

        ImGui::Spacing();
        ImGui::Spacing();

        activeLayerControls();
        sectionSeparator();
        moveContentLayerControls();
        sectionSeparator();
        fillRandomTilesControls();
    }
    ImGui::End();
}

void GuiManager::activeLayerControls() const {
    ImGui::Text("Active layer:");
    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo("##activeLayer", std::to_string(editor->getActiveLayer()).c_str())) {
        for (int i = 0; i < editor->getTileMap()->getLayers().size(); ++i) {
            const bool isSelected = editor->getActiveLayer() == i;
            if (ImGui::Selectable(std::to_string(i).c_str(), isSelected)) {
                editor->setActiveLayer(i);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::SameLine();
    if (ImGui::Button("Add layer")) {
        editor->getTileMap()->addLayer();
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove layer")) {
        ImGui::OpenPopup(REMOVE_LAYER_CONFIRMATION);
    }

    ImGui::SliderInt("Other layers opacity", reinterpret_cast<int *>(&editor->getTileMap()->getOtherLayersOpacity()), 0, 255);

    removeLayerConfirmationDialog();
}

void GuiManager::removeLayerConfirmationDialog() const {
    if (ImGui::BeginPopupModal(REMOVE_LAYER_CONFIRMATION, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Are you sure you want to delete the selected layer?\nThis action cannot be undone.");
        ImGui::Separator();
        ImGui::Spacing();

        constexpr float buttonWidth = 60.0f;
        const float spacing = ImGui::GetStyle().ItemSpacing.x;
        const float totalWidth = 2 * buttonWidth + spacing;
        ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalWidth - ImGui::GetStyle().WindowPadding.x);

        if (ImGui::Button("Yes", ImVec2(60, 0))) {
            editor->getTileMap()->removeLayer(editor->getActiveLayer());
            editor->setActiveLayer(std::max(0, editor->getActiveLayer() - 1));
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("No", ImVec2(60, 0))) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }
}

void GuiManager::moveContentLayerControls() const {
    ImGui::Text("Move content from selected layer to:");

    ImGui::SetNextItemWidth(100);
    if (ImGui::BeginCombo("##targetLayer", std::to_string(editor->getTargetLayer()).c_str())) {
        for (int i = 0; i < editor->getTileMap()->getLayers().size(); ++i) {
            const bool isSelected = (editor->getTargetLayer() == i);
            if (ImGui::Selectable(std::to_string(i).c_str(), isSelected)) {
                editor->setTargetLayer(i);
            }
            if (isSelected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }

    ImGui::SameLine();

    if (ImGui::Button("Move content")) {
        if (editor->getActiveLayer() != editor->getTargetLayer()) {
            auto &layers = editor->getTileMap()->getLayers();
            layers[editor->getTargetLayer()] = layers[editor->getActiveLayer()];
            layers[editor->getActiveLayer()] = std::vector(
                editor->getWorldHeight(),
                std::vector(editor->getWorldWidth(), editor->NO_TILE)
            );
        } else {
            ImGui::OpenPopup(MOVE_CONTENT_ERROR);
        }
    }

    moveContentLayerErrorDialog();
}

void GuiManager::moveContentLayerErrorDialog() {
    if (ImGui::BeginPopupModal(MOVE_CONTENT_ERROR, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
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

void GuiManager::fillRandomTilesControls() const {
    ImGui::Text("Fill random with selected tile:");
    ImGui::SetNextItemWidth(100);
    ImGui::InputInt("Amount: ", &editor->getAmountOfRandomTiles());
    if (ImGui::Button("Paint Random Tiles")) {
        editor->paintRandomTiles();
    }
}

void GuiManager::sectionSeparator() {
    ImGui::Spacing();
    ImGui::Spacing();
    ImGui::Separator();
}

void GuiManager::drawGuiSettings() const {
    ImGui::SetNextWindowPos(ImVec2(static_cast<float>(GetScreenWidth()) - 300, 0), ImGuiCond_Always);
    ImGui::SetNextWindowSize(ImVec2(300, static_cast<float>(GetScreenHeight())), ImGuiCond_Always);

    if (ImGui::Begin("Config", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize)) {
        if (ImGui::Button("New map")) {
            if (editor->getUnsavedChanges()) {
                ImGui::OpenPopup(NEW_MAP_WARNING);
            } else {
                editor->createNewMap();
            }
        }

        ImGui::Spacing();
        ImGui::Spacing();

        ImGui::Text("TileSet: %s", editor->getTileSetName().c_str());
        ImGui::Separator();
        if (ImGui::Button("Select TileSet")) {
            IGFD::FileDialogConfig config;
            config.path = ".";
            ImGuiFileDialog::Instance()->OpenDialog(SELECT_TILESET, "Choose File", ".png,.jpeg,.jpg,.*", config);
        }

        ImGui::InputInt("Map width", &editor->getWorldWidth());
        ImGui::InputInt("Map height", &editor->getWorldHeight());

        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        ImGui::Spacing();
        if (editor->getUnsavedChanges()) {
            ImGui::PushStyleColor(ImGuiCol_Text, IM_COL32(255, 0, 0, 255)); // Cambiar color a rojo
            ImGui::Text("TileMap: %s*", editor->getTileMap()->getTileMapName().c_str());
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Unsaved changes");
            }
            ImGui::PopStyleColor();
        } else {
            ImGui::Text("TileMap: %s", editor->getTileMap()->getTileMapName().c_str());
        }
        ImGui::Separator();

        ImGui::InputInt("Tile width", &editor->getTileWidth());
        ImGui::InputInt("Tile height", &editor->getTileHeight());

        ImGui::Spacing();
        ImGui::Spacing();

        if (ImGui::Button("Load map")) {
            showLoadMapDialog();
        }

        ImGui::SameLine();
        if (ImGui::Button("Save map")) {
            showSaveMapDialog();
        }

        if (ImGuiFileDialog::Instance()->Display(LOAD_MAP)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                editor->loadMap(
                    ImGuiFileDialog::Instance()->GetFilePathName(),
                    ImGuiFileDialog::Instance()->GetCurrentFileName()
                );
            }

            ImGuiFileDialog::Instance()->Close();
        }

        if (ImGuiFileDialog::Instance()->Display(SAVE_MAP)) {
            if (ImGuiFileDialog::Instance()->IsOk()) {
                editor->saveMap(
                    ImGuiFileDialog::Instance()->GetFilePathName(),
                    ImGuiFileDialog::Instance()->GetCurrentFileName()
                );
            }

            ImGuiFileDialog::Instance()->Close();
        }

        selectTileSetDialog();
        confirmNewMapDialog();
    }
    ImGui::End();
}

void GuiManager::selectTileSetDialog() const {
    if (ImGuiFileDialog::Instance()->Display(SELECT_TILESET)) {
        if (ImGuiFileDialog::Instance()->IsOk()) {
            editor->setTileSetPath(ImGuiFileDialog::Instance()->GetFilePathName());
            editor->setTileSetName(ImGuiFileDialog::Instance()->GetCurrentFileName());
        }

        ImGuiFileDialog::Instance()->Close();
    }
}

void GuiManager::showLoadMapDialog() {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog(LOAD_MAP, "Load Map", ".tm", config);
}

void GuiManager::showSaveMapDialog() {
    IGFD::FileDialogConfig config;
    config.path = ".";
    ImGuiFileDialog::Instance()->OpenDialog(SAVE_MAP, "Save Map As", ".tm", config);
}

void GuiManager::confirmNewMapDialog() const {
    if (editor->getUnsavedChanges()) {
        if (ImGui::BeginPopupModal(NEW_MAP_WARNING, nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
            ImGui::Text(
                "There are unsaved changes. Do you want to continue?\nThis action will discard the current changes.");
            ImGui::Separator();
            ImGui::Spacing();

            constexpr float buttonWidth = 60.0f;
            const float spacing = ImGui::GetStyle().ItemSpacing.x;
            const float totalWidth = 2 * buttonWidth + spacing;
            ImGui::SetCursorPosX(ImGui::GetWindowWidth() - totalWidth - ImGui::GetStyle().WindowPadding.x);

            if (ImGui::Button("Yes", ImVec2(buttonWidth, 0))) {
                editor->createNewMap();
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
