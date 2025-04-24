#pragma once

#include <raylib.h>
#include <vector>
#include <string>

class TileMap {
    std::string &tileSetPath;
    Texture2D &tileSet;
    std::string tileMapName;
    int widthInTiles;
    int heightInTiles;

    int tileWidth;
    int tileHeight;

    std::vector<std::vector<std::vector<int> > > layers;
    int mapWidthInTiles;
    int mapHeightInTiles;
    Rectangle srcRect = {};

    const int NO_TILE = -1;

    Vector2 mapPosition = {0, 0};

public:
    explicit TileMap(std::string &tileSetPath, Texture2D &tileSet, int widthInTiles, int heightInTiles, int tileWidth,
                     int tileHeight);

    Vector2 getPosition() const;

    void setPosition(Vector2 position);

    void initEmptyTiles(int worldWidth, int worldHeight, int layerCount);

    void draw(const int &activeLayer);

    void loadMap(const std::string &filePath, const std::string &fileName, int &mapWidth, int &mapHeight,
                 int &tileWidth, int &tileHeight, int &layerCount);

    void setTile(const float &positionX, const float &positionY, const int &tileIndex, const int &layerIndex);

    void setMapWidth(const int &value);

    void setMapHeight(const int &value);

    std::string getTileSetPath() const {
        return tileSetPath;
    }

    void setTileSetPath(const std::string &value);

    int getTileWidth() const {
        return tileWidth;
    }

    void setTileWidth(const int &value);

    int getTileHeight() const {
        return tileHeight;
    }

    void setTileHeight(const int &value);

    void updateDimens(const int &mapWidth, const int &mapHeight, const int &tileWidth, const int &tileHeight);

    std::vector<std::vector<std::vector<int>>> &getLayers() {
        return layers;
    }

    const std::vector<std::vector<std::vector<int>>> &getLayers() const {
        return layers;
    }

    void addLayer();

    void removeLayer(int layerIndex);

    void setTileMapName(const std::string &name);

    std::string getTileMapName() const {
        return tileMapName;
    };
};
