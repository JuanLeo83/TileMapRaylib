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

    Vector2 mapPosition = {0, 0};

public:
    explicit TileMap(Texture2D &tileSet, int widthInTiles, int heightInTiles, int tileWidth, int tileHeight);

    Vector2 getPosition() const;

    void setPosition(Vector2 position);

    void initEmptyTiles(int worldWidth, int worldHeight);

    void draw();

    void loadMap(const std::string &fileName, int &mapWidth, int &mapHeight, int &tileWidth, int &tileHeight);

    void setTile(float positionX, float positionY, int tileIndex);

    void setMapWidth(const int &value);

    void setMapHeight(const int &value);

    int getTileWidth() const {
        return tileWidth;
    }

    void setTileWidth(const int &value);

    int getTileHeight() const {
        return tileHeight;
    }

    void setTileHeight(const int &value);

    void updateDimens(const int &mapWidth, const int &mapHeight, const int &tileWidth, const int &tileHeight);

    std::vector<std::vector<int>> getTiles() const {
        return tiles;
    }
};
