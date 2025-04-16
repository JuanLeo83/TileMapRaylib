#include "gameState/GameState.h"

#include "test/TestScene.h"

GameState::GameState() {
    currentScene = new TestScene();
}

void GameState::update(float deltaTime) {
    currentScene->update(deltaTime);
}

void GameState::changeScene(SceneType &newSceneType) {

}
