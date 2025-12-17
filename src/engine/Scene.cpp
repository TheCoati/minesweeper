#include "Scene.h"
#include "Controller.h"

void Scene::begin() {
    this->onBegin();
}

void Scene::tick() {
    this->onTick();
}

void Scene::destroy() {
    this->onDestroy();
}
