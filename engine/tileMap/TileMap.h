#pragma once

#include <raylib.h>
#include <vector>

class TileMap {
    Texture2D &tileSet;
    int widthInTiles;
    int heightInTiles;

    int tileWidth;
    int tileHeight;

    std::vector <std::vector<int>> tiles;
    int mapWidthInTiles;
    int mapHeightInTiles;
    Rectangle srcRect = {};

    const int NO_TILE = -1;

public:
    explicit TileMap(Texture2D &tileSet, int widthInTiles, int heightInTiles, int tileWidth, int tileHeight);

    void draw();

    void loadMap(const std::string &fileName);

    std::vector<std::vector<int>> getTiles() const {
        return tiles;
    }
};
