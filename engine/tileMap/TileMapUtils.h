#pragma once
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "TileMap.h"

inline std::vector<std::vector<int> > loadMapFromFile(const std::string &fileName) {
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

inline void saveMapToFile(const TileMap &tileMap, const std::string &fileName) {
    std::ofstream file(fileName);

    if (!file.is_open()) {
        std::cerr << "Error opening the file for writing." << std::endl;
        return;
    }

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