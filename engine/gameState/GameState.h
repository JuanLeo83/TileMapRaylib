#pragma once
#include <functional>
#include <scene/Scene.h>

#include "SceneType.h"

class Scene;
using ChangeSceneCallback = std::function<void(std::shared_ptr<SceneType>)>;

class GameState {
    Scene *currentScene = nullptr;
    SceneType *nextSceneType = nullptr;
    ChangeSceneCallback changeSceneCallback;

    void setScene(Scene *scene) {
        currentScene = scene;
    }

public:
    explicit GameState();

    void update(float deltaTime) const;

    void draw() const {
        currentScene->draw();
    }

    static void changeScene(SceneType &newSceneType);
};
