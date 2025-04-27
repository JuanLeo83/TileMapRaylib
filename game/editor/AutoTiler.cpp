#include "AutoTiler.h"
#include <vector>

AutoTiler::AutoTiler(TileMap *tileMap, bool &autoTiling) : tileMap(tileMap), autoTiling(autoTiling) {
}

void AutoTiler::setTile(const float tileX, const float tileY, const int tileId, const int layer) {
    if (autoTiling) {
        setAutoTile(tileX, tileY, tileId, layer);
    } else {
        setNormalTile(tileX, tileY, tileId, layer);
    }
}

void AutoTiler::setNormalTile(const float tileX, const float tileY, const int tileId, const int layer) const {
    tileMap->setTile(tileX, tileY, tileId, layer);
}

void AutoTiler::setAutoTile(const float tileX, const float tileY, const int tileId, const int layer) {
    setAutoTileCell(tileX, tileY, tileId, layer);

    for (int dRow = -1; dRow <= 1; ++dRow) {
        for (int dCol = -1; dCol <= 1; ++dCol) {
            if (dRow == 0 && dCol == 0) continue;

            const float neighborX = tileX + dCol;
            const float neighborY = tileY + dRow;

            const auto &tiles = tileMap->getLayers()[layer];

            if (neighborY >= 0 && neighborY < tiles.size() &&
                neighborX >= 0 && neighborX < tiles[neighborY].size()) {
                const auto neighborTileId = tiles[static_cast<int>(neighborY)][static_cast<int>(neighborX)];
                if (neighborTileId != -1 && tileId != -1 && neighborTileId / 48 * 48 == tileId) {
                    setAutoTileCell(neighborX, neighborY, tileId, layer);
                } else if (tileId == -1 && neighborTileId != -1) {
                    int newId = neighborTileId / 48 * 48;
                    setAutoTileCell(neighborX, neighborY, newId, layer);
                }
            }
        }
    }
}

void AutoTiler::setAutoTileCell(const float tileX, const float tileY, const int tileId, const int layer) {
    const auto tiles = tileMap->getLayers()[layer];

    const int startRow = static_cast<int>(tileY) - 1;
    const int endRow = static_cast<int>(tileY) + 1;
    const int startCol = static_cast<int>(tileX) - 1;
    const int endCol = static_cast<int>(tileX) + 1;

    std::vector<std::vector<int> > workArea;
    std::vector<std::vector<int> > binaryArea;

    for (int row = startRow; row <= endRow; ++row) {
        std::vector<int> workRow;
        std::vector<int> binaryRow;
        for (int col = startCol; col <= endCol; ++col) {
            int value = row < 0 || row >= tiles.size() || col < 0 || col >= tiles[row].size()
                            ? tileId
                            : tiles[row][col];
            workRow.push_back(value);
            binaryRow.push_back(value != -1 && value / 48 * 48 == tileId ? 1 : 0);
        }
        workArea.push_back(workRow);
        binaryArea.push_back(binaryRow);
    }

    int bitMask = 0;
    for (int row = 0; row < binaryArea.size(); ++row) {
        for (int col = 0; col < binaryArea[row].size(); ++col) {
            if (!(row == 1 && col == 1)) {
                bitMask = bitMask << 1 | binaryArea[row][col];
            }
        }
    }

    const int newTileId = tileId + tileMasks[bitMask];
    tileMap->setTile(tileX, tileY, newTileId, layer);
}
