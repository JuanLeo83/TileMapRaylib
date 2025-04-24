#pragma once
#include <iostream>
#include <fstream>
#include <sstream>

inline std::vector<std::vector<std::vector<int>>> loadMapFromFile(const std::string &fileName, std::string &tileSetPath, int &mapWidth, int &mapHeight, int &tileWidth, int &tileHeight, int &layerCount) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for reading." << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    std::string line;
    std::vector<std::vector<std::vector<int>>> layers;

    while (std::getline(file, line)) {
        if (line.rfind("tileSetPath=", 0) == 0) {
            tileSetPath = line.substr(12);
        } else if (line.rfind("map_width=", 0) == 0) {
            mapWidth = std::stoi(line.substr(10));
        } else if (line.rfind("map_height=", 0) == 0) {
            mapHeight = std::stoi(line.substr(11));
        } else if (line.find("tile_width=", 0) == 0) {
            tileWidth = std::stoi(line.substr(11));
        } else if (line.rfind("tile_height=", 0) == 0) {
            tileHeight = std::stoi(line.substr(12));
        } else if (line.rfind("layer_count=", 0) == 0) {
            layerCount = std::stoi(line.substr(12));
            layers.resize(layerCount, std::vector(mapHeight, std::vector(mapWidth, -1)));
        } else if (line.rfind("layer=", 0) == 0) {
            int layerIndex = std::stoi(line.substr(6));
            for (int r = 0; r < mapHeight; ++r) {
                std::getline(file, line);
                std::istringstream stream(line);
                std::string value;
                int c = 0;

                while (std::getline(stream, value, ',')) {
                    layers[layerIndex][r][c++] = std::stoi(value);
                }
            }
        }
    }

    file.close();
    return layers;
}

inline void saveMapToFile(const TileMap &tileMap, const std::string &fileName) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
        return;
    }

    const auto &layers = tileMap.getLayers();
    if (layers.empty() || layers[0].empty() || layers[0][0].empty()) {
        std::cerr << "Error: TileMap layers are not properly initialized." << std::endl;
        return;
    }

    file << "tileSetPath=" << tileMap.getTileSetPath() << "\n";
    file << "map_width=" << layers[0][0].size() << "\n";
    file << "map_height=" << layers[0].size() << "\n";
    file << "tile_width=" << tileMap.getTileWidth() << "\n";
    file << "tile_height=" << tileMap.getTileHeight() << "\n";
    file << "layer_count=" << layers.size() << "\n";

    for (size_t layerIndex = 0; layerIndex < layers.size(); ++layerIndex) {
        file << "layer=" << layerIndex << "\n";
        for (const auto &row : layers[layerIndex]) {
            for (size_t col = 0; col < row.size(); ++col) {
                file << row[col];
                if (col < row.size() - 1) {
                    file << ",";
                }
            }
            file << "\n";
        }
    }

    file.close();
}