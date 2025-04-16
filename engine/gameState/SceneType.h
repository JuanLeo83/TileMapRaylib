#pragma once

#include <string>

class SceneType {
public:
    virtual ~SceneType() = default;

    virtual std::string getId() = 0;
};
