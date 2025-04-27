#include "gameState/GameState.h"

#include "editor/EditorScene.h"

GameState::GameState() {
    currentScene = new EditorScene();
}

void GameState::update(float deltaTime) const {
    currentScene->update(deltaTime);
}

void GameState::changeScene(SceneType &newSceneType) {

}
