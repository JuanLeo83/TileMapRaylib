#pragma once

#include "gameState/SceneType.h"

class TestSceneType final : public SceneType {
public:
    std::string getId() override;
};
