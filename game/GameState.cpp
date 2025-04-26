#include "gameState/GameState.h"

#include "editor/EditorScene.h"

GameState::GameState() {
    currentScene = new EditorScene();
}

void GameState::update(float deltaTime) {
    currentScene->update(deltaTime);
}

void GameState::changeScene(SceneType &newSceneType) {

}
