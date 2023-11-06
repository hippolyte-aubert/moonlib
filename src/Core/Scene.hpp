/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Scene
*/

#ifndef SCENE_HPP_
    #define SCENE_HPP_

    #include <memory>
    #include <unordered_map>

namespace ECS {
    class Scene {
        public:
            virtual void OnCreate() = 0;
            virtual void OnDestroy() = 0;
            virtual void OnActivate() = 0;
            virtual void OnDeactivate() = 0;

            virtual void Start() = 0;
            virtual void Update() = 0;
            virtual void LateUpdate() = 0;
    };

    class SceneManager {
        public:
            SceneManager();
            ~SceneManager();

            void Start();
            void Update();
            void LateUpdate();

            unsigned int AddScene(std::shared_ptr<Scene> scene);

            void SwitchScene(unsigned int id);
            void RemoveScene(unsigned int id);

        private:
            std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
            std::shared_ptr<Scene> currentScene;
            unsigned int insertedSceneId;
    };
}

#endif /* !SCENE_HPP_ */
