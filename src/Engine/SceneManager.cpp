#include "SceneManager.h"

SceneManager::~SceneManager() {
    unloadScene();
}

void SceneManager::tick() {
    if (currentScene != nullptr) {
        currentScene->tick();
    }
}

void SceneManager::switchScene(Scene *newScene) {
    if (newScene == currentScene) {
        return;
    }

    unloadScene(); // Make sure the current scene is unloaded

    currentScene = newScene;

    if (currentScene != nullptr) {
        currentScene->begin();
    }
}

void SceneManager::unloadScene() {
    if (currentScene != nullptr) {
        currentScene->destroy();

        delete currentScene;

        currentScene = nullptr;
    }
}

class SceneManager SceneManager;
