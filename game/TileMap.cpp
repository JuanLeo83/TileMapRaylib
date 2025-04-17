#include "TileMap.h"

#include <fstream>
#include <iostream>
#include <sstream>

TileMap::TileMap(Texture2D &tileSet, const int widthInTiles, const int heightInTiles, const int tileWidth,
                 const int tileHeight) : tileSet(tileSet), widthInTiles(widthInTiles), heightInTiles(heightInTiles),
                                         tileWidth(tileWidth), tileHeight(tileHeight) {
    tiles = loadMapFromFile("../assets/myMap.tm");
    mapWidthInTiles = 5;
    mapHeightInTiles = 6;
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

std::vector<std::vector<int> > TileMap::loadMapFromFile(const std::string &fileName) {
    std::vector<std::vector<int> > map;
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for reading." << std::endl;
        return map;
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream stream(line);
        std::vector<int> row;
        std::string value;

        while (std::getline(stream, value, ',')) {
            row.push_back(std::stoi(value));
        }

        map.push_back(row);
    }

    file.close();
    return map;
}

void TileMap::saveMapToFile(const std::string &fileName) const {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
        return;
    }

    for (const auto &row: tiles) {
        for (size_t col = 0; col < row.size(); ++col) {
            file << row[col];
            if (col < row.size() - 1) {
                file << ",";
            }
        }
        file << "\n";
    }

    file.close();
}
