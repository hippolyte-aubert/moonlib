/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Engine
*/

#ifndef ENGINE_HPP_
    #define ENGINE_HPP_

    #include <iostream>
    #include <raylib.h>
    #include <memory>
    #include "Coordinator.hpp"
    #include "Scene.hpp"
    #include "Asset.hpp"
    #include "../Systems/RenderingSystem.hpp"
    #include "../Systems/PhysicsSystem.hpp"
    #include "../Systems/CollisionSystem.hpp"
    #include "../Systems/EntityDataSystem.hpp"
    #include "../Systems/UserInterfaceSystem.hpp"
    #include "../Systems/AudioSystem.hpp"
    #include "../Systems/InputSystem.hpp"
    #include "Camera.hpp"

namespace ECS {
    class DefaultScene : public Scene {
        public:
            void OnCreate() override {};
            void OnDestroy() override {};
            void OnActivate() override {};
            void OnDeactivate() override {};
            void Start() override {};
            void Update() override {};
            void LateUpdate() override {};
    };

    enum Mode {
        Mode2D,
        Mode3D,
    };

    class Engine {
        public:
            Engine(std::string gameName, int width, int height, Mode mode,const std::string& assetsDirectory, bool render = true);
            ~Engine();

            void Init();
            void SetAssetsDirectory(const std::string& assetsDirectory);
            void Run();
            void Stop();
            void Start();
            void Update();
            void Render();
            void LateUpdate();
            unsigned int AddScene(std::shared_ptr<Scene> scene);
            void SwitchScene(unsigned int id);
            void RemoveScene(unsigned int id);

            // Camera
            void SetCamera(Camera camera);
            Camera& GetCamera();

            // ECS functions
            Entity CreateEntity();
            Entity CreateCustomEntity(int id);
            void DestroyEntity(Entity entity);
            bool EntityExists(Entity entity);

            template<typename T>
            void AddComponent(Entity entity, T component)
            {
                _coordinator.AddComponent<T>(entity, component);
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                _coordinator.RemoveComponent<T>(entity);
            }

            template<typename T>
            T& GetComponent(Entity entity)
            {
                return _coordinator.GetComponent<T>(entity);
            }

            std::queue<Entity> GetAvailableEntities()
            {
                return _coordinator.GetAvailableEntities();
            }

            // Utility functions
            Vector2 GetScreenSize() const;
            void SetAudioVolume(float volume);

        protected:
        private:
            bool _render;
            bool _isRunning;
            Camera _camera;
            Mode _mode;
            Coordinator _coordinator;
            std::string _assetsDirectory;
            SceneManager _sceneManager;
            AssetManager& _assetManager;
            std::shared_ptr<RenderingSystem> _renderer;
            std::shared_ptr<PhysicsSystem> _physics;
            std::shared_ptr<CollisionSystem> _collision;
            std::shared_ptr<TextSystem> _text;
            std::shared_ptr<ButtonSystem> _button;
            std::shared_ptr<AudioSystem> _audio;
            std::shared_ptr<EntityDataSystem> _entityData;

            void Render2D();
            void Render3D();
    };
}

#endif /* !ENGINE_HPP_ */
