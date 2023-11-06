/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Scene
*/

#include "Scene.hpp"
#include <iostream>

namespace ECS {
    SceneManager::SceneManager() : scenes(0), currentScene(0), insertedSceneId(0)
    {
    }

    SceneManager::~SceneManager()
    {
    }

    void SceneManager::Start()
    {
        if (currentScene) {
            currentScene->Start();
        }
    }

    void SceneManager::Update()
    {
        if (currentScene) {
            currentScene->Update();
        }
    }

    void SceneManager::LateUpdate()
    {
        if (currentScene)
            currentScene->LateUpdate();
    }

    unsigned int SceneManager::AddScene(std::shared_ptr<Scene> scene)
    {
        auto inserted = scenes.insert(std::make_pair(insertedSceneId, scene));
        insertedSceneId++;
        inserted.first->second->OnCreate();
        return insertedSceneId - 1;
    }

    void SceneManager::RemoveScene(unsigned int id)
    {
        auto it = scenes.find(id);
        if (it != scenes.end()) {
            if (currentScene == it->second)
                currentScene = nullptr;
            it->second->OnDestroy();
            scenes.erase(it);
        }
    }

    void SceneManager::SwitchScene(unsigned int id)
    {
        auto it = scenes.find(id);
        if (it != scenes.end()) {
            if (currentScene)
                currentScene->OnDeactivate();
            currentScene = it->second;
            currentScene->OnActivate();
        }
    }
}
