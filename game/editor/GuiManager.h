#pragma once

class EditorScene;

class GuiManager {
    static constexpr auto REMOVE_LAYER_CONFIRMATION = "Remove layer confirmation";
    static constexpr auto MOVE_CONTENT_ERROR = "Move content: Error";
    static constexpr auto NEW_MAP_WARNING = "New map: Warning";
    static constexpr auto SELECT_TILESET = "ChooseFileDlgKey";
    static constexpr auto LOAD_MAP = "LoadMapDlgKey";
    static constexpr auto SAVE_MAP = "SaveMapDlgKey";

    EditorScene *editor;

    void drawGuiTileSet() const;

    void drawGuiTileMap() const;

    void activeLayerControls() const;

    void removeLayerConfirmationDialog() const;

    void moveContentLayerControls() const;

    static void moveContentLayerErrorDialog();

    void fillRandomTilesControls() const;

    static void sectionSeparator();

    void drawGuiSettings() const;

    void selectTileSetDialog() const;

    static void showLoadMapDialog();

    void confirmNewMapDialog() const;

public:
    explicit GuiManager(EditorScene *testScene);

    static bool hasMouseFocus();

    void drawGui() const;

    static void showSaveMapDialog();
};
