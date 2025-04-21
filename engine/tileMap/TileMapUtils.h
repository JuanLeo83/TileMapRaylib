#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "TileMap.h"


inline std::vector<std::vector<int> > loadMapFromFile(const std::string &fileName, std::string &tileSetPath, int &mapWidth, int &mapHeight, int &tileWidth, int &tileHeight) {
    std::ifstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for reading." << std::endl;
        throw std::runtime_error("Failed to open file");
    }

    std::string line;
    std::vector<std::vector<int> > tiles;

    while (std::getline(file, line)) {
        if (line.rfind("tileSetPath=", 0) == 0) {
            tileSetPath = line.substr(12);
        } if (line.rfind("map_width=", 0) == 0) {
            mapWidth = std::stoi(line.substr(10));
        } else if (line.rfind("map_height=", 0) == 0) {
            mapHeight = std::stoi(line.substr(11));
        } else if (line.find("tile_width=", 0) == 0) {
            tileWidth = std::stoi(line.substr(11));
        } else if (line.rfind("tile_height=", 0) == 0) {
            tileHeight = std::stoi(line.substr(12));
        } else if (line == "map=") {
            while (std::getline(file, line)) {
                std::istringstream stream(line);
                std::vector<int> row;
                std::string value;

                while (std::getline(stream, value, ',')) {
                    row.push_back(std::stoi(value));
                }

                tiles.push_back(row);
            }
        }
    }

    file.close();
    return tiles;
}

inline void saveMapToFile(const TileMap &tileMap, const std::string &fileName) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
        return;
    }

    file << "tileSetPath=" << tileMap.getTileSetPath() << "\n";
    file << "map_width=" << tileMap.getTiles()[0].size() << "\n";
    file << "map_height=" << tileMap.getTiles().size() << "\n";
    file << "tile_width=" << tileMap.getTileWidth() << "\n";
    file << "tile_height=" << tileMap.getTileHeight() << "\n";
    file << "map=\n";

    for (const auto &row: tileMap.getTiles()) {
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
