#include "TileMap.h"

#include <fstream>
#include <sstream>

#include "TileMapUtils.h"

TileMap::TileMap(Texture2D &tileSet, const int widthInTiles, const int heightInTiles, const int tileWidth,
                 const int tileHeight) : tileSet(tileSet), widthInTiles(widthInTiles), heightInTiles(heightInTiles),
                                         tileWidth(tileWidth), tileHeight(tileHeight), mapWidthInTiles(0),
                                         mapHeightInTiles(0) {
    srcRect.width = tileWidth;
    srcRect.height = tileHeight;
}

Vector2 TileMap::getPosition() const {
    return mapPosition;
}

void TileMap::setPosition(const Vector2 position) {
    mapPosition = position;
}

void TileMap::initEmptyTiles(const int worldWidth, const int worldHeight) {
    mapWidthInTiles = worldWidth;
    mapHeightInTiles = worldHeight;
    tiles = std::vector(worldHeight, std::vector(worldWidth, NO_TILE));
}

void TileMap::draw() {
    for (int row = 0; row < mapHeightInTiles; ++row) {
        for (int col = 0; col < mapWidthInTiles; ++col) {
            const int tileIndex = tiles[row][col];
            if (tileIndex == NO_TILE) continue;

            const int tileRow = tileIndex / widthInTiles;
            srcRect.x = static_cast<float>((tileIndex - tileRow * widthInTiles) * tileWidth);
            srcRect.y = static_cast<float>(tileRow * tileHeight);

            DrawTextureRec(
                tileSet,
                srcRect,
                {mapPosition.x + col * tileWidth, mapPosition.y + row * tileHeight},
                WHITE
            );
        }
    }
}

void TileMap::loadMap(const std::string &fileName, int &mapWidth, int &mapHeight) {
    tiles = loadMapFromFile(fileName, mapWidthInTiles, mapHeightInTiles);
    mapWidth = mapWidthInTiles;
    mapHeight = mapHeightInTiles;
}

void TileMap::setTile(const float positionX, const float positionY, const int tileIndex) {
    const int col = static_cast<int>(positionX);
    const int row = static_cast<int>(positionY);

    if (row >= 0 && row < mapHeightInTiles && col >= 0 && col < mapWidthInTiles) {
        tiles[row][col] = tileIndex;
    }
}

void TileMap::setTileWidth(const int &value) {
    if (value == mapWidthInTiles) return;

    if (mapWidthInTiles < value) {
        for (auto &row : tiles) {
            row.resize(value, NO_TILE);
        }
    } else {
        for (auto &row : tiles) {
            row.resize(value);
        }
    }

    mapWidthInTiles = value;
}

void TileMap::setTileHeight(const int &value) {
    if (value == mapHeightInTiles) return;

    if (mapHeightInTiles < value) {
        tiles.resize(value, std::vector(mapWidthInTiles, NO_TILE));
    } else {
        tiles.resize(value);
    }

    mapHeightInTiles = value;
}
