#include "SceneManager.h"

SceneManager::~SceneManager() {
    if (currentScene) {
        currentScene->destroy();

        delete currentScene;

        currentScene = nullptr;
    }
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

    if (this->currentScene != nullptr) {
        this->currentScene->destroy();

        delete this->currentScene;

        this->currentScene = nullptr;
    }

    this->currentScene = newScene;

    if (this->currentScene != nullptr) {
        this->currentScene->begin();
    }
}

class SceneManager SceneManager;
