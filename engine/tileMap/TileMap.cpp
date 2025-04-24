#include "TileMap.h"

#include <fstream>
#include <sstream>

#include "TileMapUtils.h"

TileMap::TileMap(std::string &tileSetPath, Texture2D &tileSet, const int widthInTiles, const int heightInTiles,
                 const int tileWidth,
                 const int tileHeight) : tileSetPath(tileSetPath), tileSet(tileSet),
                                         widthInTiles(widthInTiles), heightInTiles(heightInTiles),
                                         tileWidth(tileWidth), tileHeight(tileHeight),
                                         mapWidthInTiles(0), mapHeightInTiles(0) {
    srcRect.width = static_cast<float>(tileWidth);
    srcRect.height = static_cast<float>(tileHeight);
}

Vector2 TileMap::getPosition() const {
    return mapPosition;
}

void TileMap::setPosition(const Vector2 position) {
    mapPosition = position;
}

void TileMap::initEmptyTiles(const int worldWidth, const int worldHeight, const int layerCount) {
    mapWidthInTiles = worldWidth;
    mapHeightInTiles = worldHeight;
    layers = std::vector(layerCount, std::vector(worldHeight, std::vector(worldWidth, NO_TILE)));
}

void TileMap::draw(const int &activeLayer) {
    for (int layerIndex = 0; layerIndex < layers.size(); ++layerIndex) {
        const auto &layer = layers[layerIndex];
        for (int row = 0; row < mapHeightInTiles; ++row) {
            for (int col = 0; col < mapWidthInTiles; ++col) {
                const int tileIndex = layer[row][col];
                if (tileIndex == NO_TILE) continue;

                const int tileRow = tileIndex / widthInTiles;
                srcRect.x = static_cast<float>((tileIndex - tileRow * widthInTiles) * tileWidth);
                srcRect.y = static_cast<float>(tileRow * tileHeight);

                // Aplicar transparencia si la capa está por encima de la seleccionada
                Color tint = WHITE;
                if (layerIndex > activeLayer) {
                    tint.a = 150; // Ajustar el nivel de transparencia (0-255)
                }

                DrawTextureRec(
                    tileSet,
                    srcRect,
                    {mapPosition.x + col * tileWidth, mapPosition.y + row * tileHeight},
                    tint
                );
            }
        }

        // Oscurecer la capa si está por debajo de la seleccionada
        if (layerIndex < activeLayer) {
            DrawRectangle(
                static_cast<int>(mapPosition.x),
                static_cast<int>(mapPosition.y),
                mapWidthInTiles * tileWidth,
                mapHeightInTiles * tileHeight,
                {0, 0, 0, 100} // Color negro semitransparente
            );
        }
    }
}

void TileMap::loadMap(const std::string &filePath, const std::string &fileName, int &mapWidth, int &mapHeight, int &tileWidth, int &tileHeight, int &layerCount) {
    if (tileSet.id != 0) {
        UnloadTexture(tileSet);
    }

    layers = loadMapFromFile(filePath, tileSetPath, mapWidthInTiles, mapHeightInTiles, tileWidth, tileHeight, layerCount);
    mapWidth = mapWidthInTiles;
    mapHeight = mapHeightInTiles;
    widthInTiles = tileSet.width / tileWidth;
    heightInTiles = tileSet.height / tileHeight;
    tileSet = LoadTexture(tileSetPath.c_str());
    setTileMapName(fileName);
}

void TileMap::setTile(const float &positionX, const float &positionY, const int &tileIndex, const int &layerIndex) {
    const int col = static_cast<int>(positionX);
    const int row = static_cast<int>(positionY);

    if (row >= 0 && row < mapHeightInTiles && col >= 0 && col < mapWidthInTiles && layerIndex >= 0 && layerIndex < layers.size()) {
        layers[layerIndex][row][col] = tileIndex;
    }
}

void TileMap::setMapWidth(const int &value) {
    if (value == mapWidthInTiles) return;

    for (auto &layer : layers) {
        for (auto &row : layer) {
            if (mapWidthInTiles < value) {
                row.resize(value, NO_TILE); // Expandir con NO_TILE
            } else {
                row.resize(value); // Reducir el tamaño
            }
        }
    }

    mapWidthInTiles = value;
}

void TileMap::setMapHeight(const int &value) {
    if (value == mapHeightInTiles) return;

    for (auto &layer : layers) {
        if (mapHeightInTiles < value) {
            layer.resize(value, std::vector<int>(mapWidthInTiles, NO_TILE)); // Expandir con NO_TILE
        } else {
            layer.resize(value); // Reducir el tamaño
        }
    }

    mapHeightInTiles = value;
}

void TileMap::setTileSetPath(const std::string &value) {
    if (tileSet.id != 0) {
        UnloadTexture(tileSet);
    }
    tileSetPath = value;
    tileSet = LoadTexture(tileSetPath.c_str());
    widthInTiles = tileSet.width / tileWidth;
    heightInTiles = tileSet.height / tileHeight;
}

void TileMap::setTileWidth(const int &value) {
    if (value == tileWidth) return;

    tileWidth = value;
    srcRect.width = tileWidth;
    widthInTiles = tileSet.width / tileWidth;
}

void TileMap::setTileHeight(const int &value) {
    if (value == tileHeight) return;

    tileHeight = value;
    srcRect.height = tileHeight;
    heightInTiles = tileSet.height / tileHeight;
}

void TileMap::updateDimens(const int &mapWidth, const int &mapHeight, const int &tileWidth, const int &tileHeight) {
    setMapWidth(mapWidth);
    setMapHeight(mapHeight);
    setTileWidth(tileWidth);
    setTileHeight(tileHeight);
}

void TileMap::addLayer() {
    layers.emplace_back(mapHeightInTiles, std::vector(mapWidthInTiles, NO_TILE));
}

void TileMap::removeLayer(const int layerIndex) {
    if (layers.size() > 1 && layerIndex >= 0 && layerIndex < layers.size()) {
        layers.erase(layers.begin() + layerIndex);
    }
}

void TileMap::setTileMapName(const std::string &name) {
    tileMapName = name;
}

void TileMap::floodFill(int x, int y, int layer, int newTile) {
    if (x < 0 || x >= mapWidthInTiles || y < 0 || y >= mapHeightInTiles) return;

    int oldTile = layers[layer][y][x];
    if (oldTile == newTile) return;

    std::queue<std::pair<int, int>> queue;
    queue.push({x, y});

    while (!queue.empty()) {
        auto [cx, cy] = queue.front();
        queue.pop();

        if (cx < 0 || cx >= mapWidthInTiles || cy < 0 || cy >= mapHeightInTiles) continue;
        if (layers[layer][cy][cx] != oldTile) continue;

        layers[layer][cy][cx] = newTile;

        queue.push({cx + 1, cy});
        queue.push({cx - 1, cy});
        queue.push({cx, cy + 1});
        queue.push({cx, cy - 1});
    }
}
