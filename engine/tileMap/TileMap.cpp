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
                {static_cast<float>(col * tileWidth), static_cast<float>(row * tileHeight)},
                WHITE
            );
        }
    }
}

void TileMap::loadMap(const std::string &fileName) {
    tiles = loadMapFromFile(fileName, mapWidthInTiles, mapHeightInTiles);
}
