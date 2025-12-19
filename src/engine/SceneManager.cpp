#include "SceneManager.h"

SceneManager::~SceneManager() {
    unloadScene();
}

void SceneManager::tick() {
    if (this->currentScene != nullptr) {
        this->currentScene->tick();
    }
}

void SceneManager::switchScene(Scene *newScene) {
    if (newScene == this->currentScene) {
        return;
    }

    unloadScene(); // Make sure current scene is unloaded

    this->currentScene = newScene;

    if (this->currentScene != nullptr) {
        this->currentScene->begin();
    }
}

void SceneManager::unloadScene() {
    if (this->currentScene != nullptr) {
        this->currentScene->destroy();

        delete this->currentScene;

        this->currentScene = nullptr;
    }
}

class SceneManager SceneManager;
