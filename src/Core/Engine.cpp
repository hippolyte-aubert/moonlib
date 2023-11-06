/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Engine
*/

#include "Engine.hpp"

namespace ECS {
    Engine::Engine(std::string gameName, int width, int height, Mode mode,const std::string& assetsDirectory, bool render) : _render(render), _assetManager(AssetManager::GetInstance()), _mode(mode), _camera(mode == Mode2D ? CameraType::CAMERA_2D : CameraType::CAMERA_3D)
    {
        _isRunning = true;
        if (_render) {
            InitWindow(width, height, gameName.c_str());
            InitAudioDevice();
            SetTargetFPS(60);
            _assetManager.bulkLoadAssets(assetsDirectory);
        }
    }

    Engine::~Engine()
    {
        if (_render) {
            // _assetManager.unloadAllAssets();
            CloseAudioDevice();
            CloseWindow();
        }
    }

    void Engine::Init()
    {
        _coordinator.Init();
        _coordinator.RegisterComponent<Transform2D>();
        _coordinator.RegisterComponent<Renderer2D>();
        _coordinator.RegisterComponent<RigidBody2D>();
        _coordinator.RegisterComponent<Gravity>();
        _coordinator.RegisterComponent<Collision2D>();
        _coordinator.RegisterComponent<Text>();
        _coordinator.RegisterComponent<Button>();
        _coordinator.RegisterComponent<Audio>();
        _coordinator.RegisterComponent<EntityData>();

        _renderer = _coordinator.RegisterSystem<RenderingSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<Transform2D>());
            signature.set(_coordinator.GetComponentType<Renderer2D>());
            _coordinator.SetSystemSignature<RenderingSystem>(signature);
        }

        _physics = _coordinator.RegisterSystem<PhysicsSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<Transform2D>());
            signature.set(_coordinator.GetComponentType<RigidBody2D>());
            signature.set(_coordinator.GetComponentType<Gravity>());
            _coordinator.SetSystemSignature<PhysicsSystem>(signature);
        }

        _collision = _coordinator.RegisterSystem<CollisionSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<Transform2D>());
            signature.set(_coordinator.GetComponentType<Collision2D>());
            signature.set(_coordinator.GetComponentType<Renderer2D>());
            _coordinator.SetSystemSignature<CollisionSystem>(signature);
        }

        _entityData = _coordinator.RegisterSystem<EntityDataSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<EntityData>());
            signature.set(_coordinator.GetComponentType<Renderer2D>());
            _coordinator.SetSystemSignature<EntityDataSystem>(signature);
        }

        _text = _coordinator.RegisterSystem<TextSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<Text>());
            _coordinator.SetSystemSignature<TextSystem>(signature);
        }

        _button = _coordinator.RegisterSystem<ButtonSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<Button>());
            _coordinator.SetSystemSignature<ButtonSystem>(signature);
        }

        _audio = _coordinator.RegisterSystem<AudioSystem>();
        {
            Signature signature;
            signature.set(_coordinator.GetComponentType<Audio>());
            _coordinator.SetSystemSignature<AudioSystem>(signature);
        }
    }

    void Engine::SetAssetsDirectory(const std::string& assetsDirectory)
    {
        _assetsDirectory = assetsDirectory;
    }

    void Engine::Run()
    {
        while (_render ? !WindowShouldClose() && _isRunning : _isRunning) {
            Update();
            if (_render) Render();
            LateUpdate();
        }
    }

    void Engine::Stop()
    {
        _isRunning = false;
    }

    void Engine::Start()
    {
        _sceneManager.Start();

        if (_render) {
            _renderer->Start(_coordinator, _assetManager);
            _text->Start(_coordinator, _assetManager);
            _button->Start(_coordinator, _assetManager);
            _audio->Start(_coordinator, _assetManager);
        }
        _physics->Start(_coordinator);
        _collision->Start(_coordinator);
        _entityData->Start(_coordinator);
    }

    void Engine::Update()
    {
        if (_render) _audio->Update(_coordinator, _assetManager);
        _physics->Update(_coordinator);
        _collision->Update(_coordinator);
        _entityData->Update(_coordinator);
        _sceneManager.Update();
    }

    void Engine::Render2D()
    {
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode2D(_camera.GetCamera2D());
                _renderer->Update(_coordinator, _assetManager);
                _collision->OnGizmoDraw(_coordinator);
            EndMode2D();
            _text->Update(_coordinator, _assetManager);
            _button->Update(_coordinator, _assetManager);
        EndDrawing();
    }

    void Engine::Render3D()
    {
        BeginDrawing();
            ClearBackground(BLACK);
            BeginMode3D(_camera.GetCamera3D());
                _renderer->Update(_coordinator, _assetManager);
                _collision->OnGizmoDraw(_coordinator);
            EndMode3D();
            _text->Update(_coordinator, _assetManager);
            _button->Update(_coordinator, _assetManager);
        EndDrawing();
    }

    void Engine::Render()
    {
        _mode ? Render3D() : Render2D();
    }

    void Engine::LateUpdate()
    {
        _sceneManager.LateUpdate();
    }

    unsigned int Engine::AddScene(std::shared_ptr<Scene> scene)
    {
        return _sceneManager.AddScene(scene);
    }

    void Engine::SwitchScene(unsigned int id)
    {
        _renderer->Clear();
        _collision->Clear();
        _physics->Clear();
        _entityData->Clear();
        _text->Clear();
        _button->Clear();
        _audio->Clear();
        _sceneManager.SwitchScene(id);
        Start();
    }

    void Engine::RemoveScene(unsigned int id)
    {
        _sceneManager.RemoveScene(id);
    }

    void Engine::SetCamera(Camera camera)
    {
        if ((camera.Type() == CameraType::CAMERA_2D && _mode == Mode3D) ||
            (camera.Type() == CameraType::CAMERA_3D && _mode == Mode2D))
            return;
        _camera = camera;
    }

    Camera& Engine::GetCamera()
    {
        return _camera;
    }

    void Engine::SetAudioVolume(float volume)
    {
        SetMasterVolume(volume);
    }

    Vector2 Engine::GetScreenSize() const
    {
        return {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
    }

    Entity Engine::CreateEntity()
    {
        return _coordinator.CreateEntity();
    }

    Entity Engine::CreateCustomEntity(int id)
    {
        return _coordinator.CreateCustomEntity(id);
    }

    void Engine::DestroyEntity(Entity entity)
    {
        _coordinator.DestroyEntity(entity);
        std::cout << "Destroyed entity " << entity << std::endl;
    }

    bool Engine::EntityExists(Entity entity)
    {
        return _coordinator.EntityExists(entity);
    }
}