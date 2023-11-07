#ifndef MOONLIB_H_
#define MOONLIB_H_

#include <raylib.h>
#include <raymath.h>
#include <iostream>
#include <set>
#include <cmath>
#include <functional>
#include <unordered_map>
#include <list>
#include <filesystem>
#include <any>
#include <optional>
#include <cstdint>
#include <cassert>
#include <memory>
#include <bitset>
#include <queue>
#include <array>
#include <algorithm>
#include <vector>

namespace MLIB {

    //** ECS System **//

    // ---------------------------------
    // Entity Manager
    // ---------------------------------
    #define MAX_COMPONENTS 128
    #define MAX_ENTITIES 100
    #define MAX_SIZE_TAB 100001

    using Signature = std::bitset<MAX_COMPONENTS>;
    using Entity = std::uint32_t;

    class EntityManager {
        public:
            EntityManager()
            {
                _livingEntityCount = 0;

                for (std::uint32_t id = 0; id < MAX_ENTITIES; ++id) {
                    std::uint32_t id_rng = rand() % 10000;
                    Entity entity(id_rng);
                    _availableEntities.push(entity);
                }
            }

            Entity CreateCustomEntity(int id)
            {
                uint32_t id_casted = static_cast<uint32_t>(id);

                Entity entity(id_casted);
                std::cout << "Entity " << entity << " created" << std::endl;
                ++_livingEntityCount;
                return entity;
            }

            Entity CreateEntity()
            {
                assert(_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

                Entity id = _availableEntities.front();
                std::cout << "Entity " << id << " created" << std::endl;
                _availableEntities.pop();
                ++_livingEntityCount;
                return id;
            }

            void DestroyEntity(Entity entity)
            {
                _signatures[entity].reset();
                _availableEntities.push(entity);
                --_livingEntityCount;
            }

            void SetSignature(Entity entity, Signature signature)
            {
                _signatures[entity] = signature;
            }

            Signature GetSignature(Entity entity)
            {
                return _signatures[entity];
            }

            std::queue<Entity> GetAvailableEntities() const
            {
                return _availableEntities;
            }

            bool EntityExists(Entity entity)
            {
                return _signatures[entity].any();
            }

        private:
            std::queue<Entity> _availableEntities;
            std::array<Signature, MAX_SIZE_TAB> _signatures;
            std::uint32_t _livingEntityCount;
    };

    // ---------------------------------
    // Component System
    // ---------------------------------
    using ComponentType = std::uint8_t;

    class IComponentArray {
        public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
        public:
        void InsertData(Entity entity, T component)
        {
            assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component already added to this entity.");

            size_t newIndex = mSize;
            mEntityToIndexMap[entity] = newIndex;
            mIndexToEntityMap[newIndex] = entity;
            mComponentArray[newIndex] = component;
            ++mSize;
        }

        void RemoveData(Entity entity)
        {
            assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Component does not exist in this entity");
            
            size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
            size_t indexOfLastElement = mSize - 1;
            mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

            Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
            mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            mEntityToIndexMap.erase(entity);
            mIndexToEntityMap.erase(indexOfLastElement);

            --mSize;
        }

        T& GetData(Entity entity)
        {
            //assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

            return mComponentArray[mEntityToIndexMap[entity]];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
            {
                RemoveData(entity);
            }
        }
        private:
            std::array<T, MAX_ENTITIES> mComponentArray;
            std::unordered_map<Entity, size_t> mEntityToIndexMap;
            std::unordered_map<size_t, Entity> mIndexToEntityMap;
            size_t mSize;
    };

    class ComponentManager
    {
        public:
            ~ComponentManager() = default;
        
            template<typename T>
            void RegisterComponent()
            {
                std::string typeName = typeid(T).name();

                std::cout << "Registering component type: " << typeName << std::endl;

                assert(_componentTypes.find(typeName) == _componentTypes.end() && "Registering component type more than once.");

                _componentTypes.insert({typeName, _nextComponentType});
                _componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
                ++_nextComponentType;
            }

            template<typename T>
            ComponentType GetComponentType()
            {
                std::string typeName = typeid(T).name();

                assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

                return _componentTypes[typeName];
            }

            template<typename T>
            void AddComponent(Entity entity, T component)
            {
                std::string typeName = typeid(T).name();
                GetComponentArray<T>()->InsertData(entity, component);
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                GetComponentArray<T>()->RemoveData(entity);
            }

            template<typename T>
            T& GetComponent(Entity entity)
            {
                return GetComponentArray<T>()->GetData(entity);
            }

            void EntityDestroyed(Entity entity)
            {
                for (auto const& pair : _componentArrays)
                {
                    auto const& component = pair.second;

                    component->EntityDestroyed(entity);
                }
            }

        private:
            std::unordered_map<std::string , ComponentType> _componentTypes;
            std::unordered_map<std::string , std::shared_ptr<IComponentArray>> _componentArrays;
            ComponentType _nextComponentType;

            template<typename T>
            std::shared_ptr<ComponentArray<T>> GetComponentArray()
            {
                std::string typeName = typeid(T).name();

                assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

                return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
            }
    };

    // ---------------------------------
    // System Manager
    // ---------------------------------
    class System {
        public:
            void Clear()
            {
                entities.clear();
            }
            std::set<Entity> entities;
    };

    class SystemManager
    {
        public:
            template<typename T>
            std::shared_ptr<T> RegisterSystem()
            {
                std::string typeName = typeid(T).name();

                assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

                auto system = std::make_shared<T>();
                _systems.insert({typeName, system});
                return system;
            }

            template<typename T>
            void SetSignature(Signature signature)
            {
                std::string typeName = typeid(T).name();
                std::cout << "Setting signature for " << typeName << std::endl;

                assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

                _signatures.insert({typeName, signature});
            }

            void EntityDestroyed(Entity entity)
            {
                for (auto const& pair : _systems)
                {
                    auto const& system = pair.second;

                    system->entities.erase(entity);
                }
            }

            void EntitySignatureChanged(Entity entity, Signature entitySignature)
            {
                for (auto const& pair : _systems)
                {
                    auto const& type = pair.first;
                    auto const& system = pair.second;
                    auto const& systemSignature = _signatures[type];

                    if ((entitySignature & systemSignature) == systemSignature)
                    {
                        system->entities.insert(entity);
                    }
                    else
                    {
                        system->entities.erase(entity);
                    }
                }
            }

        private:
            std::unordered_map<std::string, Signature> _signatures{};
            std::unordered_map<std::string, std::shared_ptr<System>> _systems{};
    };

    // ---------------------------------
    // Coordinator
    // ---------------------------------
    class Coordinator {
        public:
            void Init()
            {
                _componentManager = std::make_unique<ComponentManager>();
                _entityManager = std::make_unique<EntityManager>();
                _systemManager = std::make_unique<SystemManager>();
            }

            Entity CreateEntity()
            {
                return _entityManager->CreateEntity();
            }

            Entity CreateCustomEntity(int id)
            {
                return _entityManager->CreateCustomEntity(id);
            }

            void DestroyEntity(Entity entity)
            {
                _entityManager->DestroyEntity(entity);
                _componentManager->EntityDestroyed(entity);
                _systemManager->EntityDestroyed(entity);
                std::cout << "Entity " << entity << " destroyed" <<std::endl;
            }

            bool EntityExists(Entity entity)
            {
                return _entityManager->EntityExists(entity);
            }

            template<typename T>
            void RegisterComponent()
            {
                _componentManager->RegisterComponent<T>();
            }

            template<typename T>
            void AddComponent(Entity entity, T component)
            {
                _componentManager->AddComponent<T>(entity, component);

                auto signature = _entityManager->GetSignature(entity);
                signature.set(_componentManager->GetComponentType<T>(), true);

                _entityManager->SetSignature(entity, signature);
                _systemManager->EntitySignatureChanged(entity, signature);
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                _componentManager->RemoveComponent<T>(entity);

                auto signature = _entityManager->GetSignature(entity);
                signature.set(_componentManager->GetComponentType<T>(), false);

                _entityManager->SetSignature(entity, signature);
                _systemManager->EntitySignatureChanged(entity, signature);
            }

            template<typename T>
            T& GetComponent(Entity entity)
            {
                return _componentManager->GetComponent<T>(entity);
            }

            template<typename T>
            ComponentType GetComponentType()
            {
                return _componentManager->GetComponentType<T>();
            }

            template<typename T>
            std::shared_ptr<T> RegisterSystem()
            {
                return _systemManager->RegisterSystem<T>();
            }

            template<typename T>
            void SetSystemSignature(Signature signature)
            {
                _systemManager->SetSignature<T>(signature);
            }

            std::queue<Entity> GetAvailableEntities()
            {
                return _entityManager->GetAvailableEntities();
            }

        private:
            std::unique_ptr<ComponentManager> _componentManager;
            std::unique_ptr<EntityManager> _entityManager;
            std::unique_ptr<SystemManager> _systemManager;
    };

    //** ECS Components **//

    // ---------------------------------
    // Physics Components
    // ---------------------------------
    struct Transform3D {
        Vector3 position;
        Quaternion rotation;
        Vector3 scale;
    };

    struct Transform2D {
        Vector2 position;
        float rotation;
        Vector2 scale;
    };

    struct Gravity {
        float force;
        bool enabled;
    };

    struct RigidBody {
        Vector3 velocity;
        Vector3 force;
        float mass;
    };

    struct RigidBody2D {
        Vector2 velocity;
        Vector2 force;
        float mass;
    };

    // ---------------------------------
    // Rendering Components
    // ---------------------------------
    struct Renderer2D {
        std::string textureName;
        Color tint = WHITE;
        bool draw = true;
        bool textureLoaded = false;
        Texture2D texture = {0};
        int zIndex = 0;
        Vector2 origin = {0, 0};
        bool isAnimated = false;
        int nbFrames = 1;
        int framesSpeed = 0;
        int currentFrame = 0;
        int currentFrameCounter = 0;
        bool animate = false;
        bool loopAnimation = false;
        bool animateOnStart = false;
        bool destroyOnAnimationEnd = false;
        Rectangle sourceRec = {0};
    };

    // ---------------------------------
    // Collision Components
    // ---------------------------------
    struct Collision2D {
        bool isTrigger = false;
        Rectangle rect = {0, 0, 0, 0};
        bool enabled = true;
        bool debug = false;
        std::string tag = "default";
        bool collided = false;
        bool trigger = false;
        bool colliderSet = false;

        // TODO Resolve when two entity collide
        // void Solve(std::set<Entity> entities, Coordinator &coordinator, Entity self)
        // {
        //     for (auto const &entity : entities)
        //     {
        //         auto &other = coordinator.GetComponent<Collision2D>(entity);

        //         if (entity == self)
        //             continue;
        //         float xDistance = std::pow((rect.x + rect.width /2 ) - (other.rect.x + other.rect.width /2) , 2);
        //         float yDistance = std::pow((rect.y + rect.height /2 ) - (other.rect.y + other.rect.height /2) , 2);
        //         float distance = std::sqrt(xDistance + yDistance);

        //         if (IsColliding(other))
        //         {
                    // collided = true;
        //         }
        //     }
        // }

        bool IsColliding(Collision2D other)
        {
            if (CheckCollisionRecs(rect, other.rect) && enabled && other.enabled)
                return true;
            return false;
        }

        bool IsTriggering(Collision2D other)
        {
            if (CheckCollisionRecs(rect, other.rect) && (isTrigger || other.isTrigger))
                return true;
            return false;
        }
    };

    // ---------------------------------
    // Audio Components
    // ---------------------------------
    enum AudioType {
        SOUND,
        MUSIC,
        UNDEFINED
    };

    struct Audio {
        std::string audioName;
        AudioType type;
        bool loopMusic = false;
        bool playOnStart = false;
        bool playing = false;
        bool isLoaded = false;
        Sound sound = {0};
        Music music = {0};

        void PlayAudio()
        {
            if (type == AudioType::SOUND) {
                PlaySound(sound);
                std::cout << "play sound" << std::endl;
                playing = true;
            } else {
                PlayMusicStream(music);
                std::cout << "play music" << std::endl;
                playing = true;
            }
        }

        void StopAudio()
        {
            if (type == AudioType::SOUND) {
                if (IsSoundPlaying(sound)) {
                    StopSound(sound);
                    playing = false;
                }
            } else {
                if (IsMusicStreamPlaying(music)) {
                    StopMusicStream(music);
                    playing = false;
                }
            }
            std::cout << "stop audio" << std::endl;
        }

        void PauseAudio()
        {
            if (type == AudioType::SOUND) {
                if (IsSoundPlaying(sound)) {
                    PauseSound(sound);
                    playing = false;
                }
            } else {
                if (IsMusicStreamPlaying(music)) {
                    PauseMusicStream(music);
                    playing = false;
                }
            }
        }

        void ResumeAudio()
        {
            if (type == AudioType::SOUND) {
                if (!IsSoundPlaying(sound)) {
                    ResumeSound(sound);
                    playing = true;
                }
            } else {
                if (!IsMusicStreamPlaying(music)) {
                    ResumeMusicStream(music);
                    playing = true;
                }
            }
        }

        void SetVolume(float volume)
        {
            SetSoundVolume(sound, volume);
            SetMusicVolume(music, volume);
        }
    };

    // ---------------------------------
    // UserInterface Components
    // ---------------------------------
    struct Text {
        std::string text;
        std::string fontName;
        Vector2 position;
        float fontSize = 12;
        float spacing = 1;
        Color color = WHITE;
        Font font = {0};
        bool fontLoaded = false;
    };

    struct Button {
        Vector2 position;
        float width;
        float height;
        Text buttonText;
        Color baseColor = WHITE;
        Color overColor = GRAY;
        Color clickColor = DARKGRAY;
        bool rounded = false;
        float roundness = 1.0f;
        Rectangle rect = {0, 0, 0, 0};
        bool set = false;

        void OnClick(std::function<void()> callback)
        {
            static Color storeColor = baseColor;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rect)) {
                callback();
                baseColor = clickColor;
            }
        }

        void OnOver()
        {
            static Color storeColor = baseColor;
            if (CheckCollisionPointRec(GetMousePosition(), rect)) {
                baseColor = overColor;
            } else {
                baseColor = storeColor;
            }
        }
    };

    // TODO Remove this component after defense
    // ---------------------------------
    // Entity Data components
    // ---------------------------------
    struct EntityData {
        int HP;
        unsigned int DMG;
        unsigned int LVL;

        bool isAlive() {
            if (HP < 0)
                return false;
            return true;
        }
    };

    //** Scene System **//

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

    class SceneManager {
        public:
            SceneManager() : scenes(0), currentScene(0), insertedSceneId(0) {}
            ~SceneManager() {};

            void Start()
            {
                if (currentScene) currentScene->Start();
            }

            void Update()
            {
                if (currentScene) currentScene->Update();
            }

            void LateUpdate()
            {
                if (currentScene) currentScene->LateUpdate();
            }

            unsigned int AddScene(std::shared_ptr<Scene> scene)
            {
                auto inserted = scenes.insert(std::make_pair(insertedSceneId, scene));
                insertedSceneId++;
                inserted.first->second->OnCreate();
                return insertedSceneId - 1;
            }

            void SwitchScene(unsigned int id)
            {
                auto it = scenes.find(id);
                if (it != scenes.end()) {
                    if (currentScene)
                        currentScene->OnDeactivate();
                    currentScene = it->second;
                    currentScene->OnActivate();
                }
            }
            void RemoveScene(unsigned int id)
            {
                auto it = scenes.find(id);
                if (it != scenes.end()) {
                    if (currentScene == it->second)
                        currentScene = nullptr;
                    it->second->OnDestroy();
                    scenes.erase(it);
                }
            }

        private:
            std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
            std::shared_ptr<Scene> currentScene;
            unsigned int insertedSceneId;
    };

    //** Asset Manager **//
    enum AssetType {
        TEXTURE,
        FONT,
        AUDIO,
        MODEL,
        // ANIMATIONS, // TODO
        // SHADERS, // TODO
        UNKNOWN
    };

    struct Asset {
        std::string name;
        std::string path;
        AssetType type;
        int refCount;
        std::any assetData;

        template<typename T>
        T GetData()
        {
            if (std::is_same_v<T, Texture2D> && type == AssetType::TEXTURE) {
                return std::any_cast<T>(assetData);
            } else if (std::is_same_v<T, Font> && type == AssetType::FONT) {
                return std::any_cast<T>(assetData);
            } else if (std::is_same_v<T, Audio> && type == AssetType::AUDIO) {
                return std::any_cast<T>(assetData);
            } else if (std::is_same_v<T, Model> && type == AssetType::MODEL) {
                return std::any_cast<T>(assetData);
            } else {
                throw std::invalid_argument("Invalid asset type");
            }
        }

        template<typename T>
        void SetData(const T& newData)
        {
            if (std::is_same_v<T, Texture2D> && type == AssetType::TEXTURE) {
                assetData = newData;
            } else if (std::is_same_v<T, Font> && type == AssetType::FONT) {
                assetData = newData;
            } else if (std::is_same_v<T, Audio> && type == AssetType::AUDIO) {
                assetData = newData;
            } else if (std::is_same_v<T, Model> && type == AssetType::MODEL) {
                assetData = newData;
            } else {
                throw std::invalid_argument("Invalid asset type");
            }
        }
    };

    // Custom audio loading function because raylib uses both sound and music
    // and the asset manager can't differentiate them just from a file extension
    Audio LoadAudio(const std::string& path)
    {
        return Audio(
            "audio",
            AudioType::UNDEFINED,
            false,
            false,
            false,
            false,
            LoadSound(path.c_str()),
            LoadMusicStream(path.c_str())
        );
    }

    class AssetManager {
        public:
            static AssetManager& GetInstance() {
                if (_instance == nullptr)
                    _instance = new AssetManager();
                return *_instance;
            }

            template<typename T>
            void loadAsset(std::string name, std::string path,AssetType type)
            {
                T loadedAsset;

                if (assets.find(name) != assets.end()) {
                    assets[name].refCount++;
                } else {
                    if constexpr (std::is_same_v<T, Texture2D>) {
                        loadedAsset = LoadTexture(path.c_str());
                    } else if constexpr (std::is_same_v<T, Font>) {
                        loadedAsset = LoadFontEx(path.c_str(), 100, 0, 250);
                    } else if constexpr (std::is_same_v<T, Audio>) {
                        loadedAsset = LoadAudio(path.c_str());
                    } else if constexpr (std::is_same_v<T, Model>) {
                        loadedAsset = LoadModel(path.c_str());
                    }

                    Asset asset;
                    asset.name = name;
                    asset.path = path;
                    asset.type = type;
                    asset.refCount = 1;
                    asset.SetData(loadedAsset);
                    assets[name] = asset;
                }
            }

            void unloadAsset(std::string name)
            {
                if (assets.find(name) != assets.end()) {
                    Asset& asset = assets[name];
                    asset.refCount--;
                    if (asset.refCount <= 0) {
                        if (asset.type == AssetType::TEXTURE)
                            UnloadTexture(asset.GetData<Texture2D>());
                        else if (asset.type == AssetType::FONT)
                            UnloadFont(asset.GetData<Font>());
                        else if (asset.type == AssetType::AUDIO)
                            UnloadSound(asset.GetData<Sound>());
                        else if (asset.type == AssetType::MODEL)
                            UnloadModel(asset.GetData<Model>());
                        assets.erase(name);
                    }
                } else
                    std::cerr << "Asset " << name << " not found" << std::endl;
            }

            std::optional<Asset*> getAsset(const std::string& name)
            {
                if (assets.find(name) != assets.end()) {
                    std::cout << "Asset " << name << " found" << std::endl;
                    return &assets[name];
                }
                return std::nullopt;
            }

            std::unordered_map<std::string, Asset>& getAssets()
            {
                return assets;
            }

            // void cacheAsset(std::string name, std::string path,AssetType type);
            // void uncacheAsset(std::string name);
            // Asset* getCachedAsset(std::string name);

            // void clearCache();
            void clearAssets()
            {
                assets.clear();
            }

            void bulkLoadAssets(const std::string& assetsDirectory)
            {
                for (const auto& entry : std::filesystem::recursive_directory_iterator(assetsDirectory)) {
                    std::string path = entry.path().generic_string();
                    std::string name = path.substr(path.find_last_of('/') + 1);
                    AssetType type = getAssetExtension(path);
                    if (type == UNKNOWN)
                        continue;
                    else if (type == AssetType::TEXTURE)
                        loadAsset<Texture2D>(name, path, getAssetExtension(path));
                    else if (type == AssetType::FONT)
                        loadAsset<Font>(name, path, getAssetExtension(path));
                    else if (type == AssetType::AUDIO)
                        loadAsset<Audio>(name, path, getAssetExtension(path));
                    else if (type == AssetType::MODEL)
                        loadAsset<Model>(name, path, getAssetExtension(path));
                }
            }

            void unloadAllAssets()
            {
                for (auto& asset : assets)
                    unloadAsset(asset.first);
                assets.clear();
            }

            AssetType getAssetExtension(const std::string& assetPath)
            {
                std::string extension = assetPath.substr(assetPath.find_last_of('.') + 1);
                if (extension == "png")
                    return TEXTURE;
                else if (extension == "ttf" || extension == "otf")
                    return FONT;
                else if (extension == "wav" || extension == "mp3" || extension == "ogg")
                    return AUDIO;
                else if (extension == "obj" || extension == "gltf")
                    return MODEL;
                else
                    return UNKNOWN;
            }

        private:
            static AssetManager* _instance;
            std::unordered_map<std::string, Asset> assets;
            // std::unordered_map<std::string, Asset> cache;
            // std::list<std::string> cacheUsageOrder;

            AssetManager() {}
            AssetManager(AssetManager const &) = delete;
            AssetManager& operator=(AssetManager const &) = delete;
    };

    AssetManager* AssetManager::_instance = nullptr;

    //** Camera System **//
    enum ProjectionType {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC = 1
    };

    enum CameraType {
        CAMERA_2D = 0,
        CAMERA_3D = 1
    };

    enum CameraMode {
        CUSTOM = 0,
        FREE = 1,
        ORBITAL = 2,
        FIRST_PERSON = 3,
        THIRD_PERSON = 4
    };
    class Camera {
        public:
            Camera(CameraType type)
            {
                if (type == CAMERA_2D) {
                    _camera2D = {
                        .offset = {0, 0},
                        .target = {0, 0},
                        .rotation = 0,
                        .zoom = 1.0f,
                    };
                    _updateCamera = true;
                    _camera3D = {0};
                }

                if (type == CAMERA_3D) {
                    _camera3D = {
                        .position = {0, 0, 0},
                        .target = {0, 0, 0},
                        .up = {0, 1, 0},
                        .fovy = 70,
                        .projection = PERSPECTIVE,
                    
                    };
                    _updateCamera = true;
                    _camera2D = {0};
                }

                _type = type;
            }

            Camera(Vector2 target, Vector2 offset, float rotation, float zoom)
            {
                _camera2D = {0};
                _camera2D.target = target;
                _camera2D.offset = offset;
                _camera2D.rotation = rotation;
                _camera2D.zoom = zoom;
                _type = CAMERA_2D;
            }

            Camera(Vector3 position, Vector3 target, Vector3 up, float fovy, ProjectionType projection, CameraMode cameraMode)
            {
                _camera3D = {0};
                _camera3D.position = position;
                _camera3D.target = target;
                _camera3D.up = up;
                _camera3D.fovy = fovy;
                _camera3D.projection = projection;
                _type = CAMERA_3D;
                _mode = cameraMode;
            }

            Camera(Camera2D camera)
            {
                _camera2D = camera;
                _type = CAMERA_2D;
            }

            Camera(Camera3D camera)
            {
                _camera3D = camera;
                _type = CAMERA_3D;
            }

            ~Camera() {};

            Camera2D GetCamera2D() {return _camera2D;};
            Camera3D GetCamera3D() {return _camera3D;};

            void Type(CameraType type) {_type = type;};
            CameraType Type() {return _type;};

            void Mode(CameraMode mode) {_mode = mode;};
            CameraMode Mode() {return _mode;};

            void UpdateCamera() {_updateCamera = true;};
            bool IsCameraUpdating() {return _updateCamera;};

        private:
            Camera2D _camera2D;
            Camera3D _camera3D;
            CameraType _type;
            CameraMode _mode;
            bool _updateCamera = true;
    };

    //** ECS Systems **//

    // ---------------------------------
    // Rendering System
    // ---------------------------------
    class RenderingSystem : public System {
        public:
            RenderingSystem() = default;
            ~RenderingSystem() = default;

            void Start(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);
                    auto &transform = coordinator.GetComponent<Transform2D>(entity);

                    auto assetOpt = assetManager.getAsset(renderer2D.textureName);
                    if (assetOpt) renderer2D.texture = assetOpt.value()->GetData<Texture2D>();
                    else std::cerr << "Texture " << renderer2D.textureName << " not found" << std::endl;

                    renderer2D.texture.width *= transform.scale.x;
                    renderer2D.texture.height *= transform.scale.y;
                    renderer2D.origin = {0, 0};

                    if (renderer2D.isAnimated) {
                        renderer2D.sourceRec.width = renderer2D.texture.width / renderer2D.nbFrames;
                        renderer2D.sourceRec.height = renderer2D.texture.height;
                        if (renderer2D.animateOnStart) renderer2D.animate = true;
                    } else {
                        renderer2D.sourceRec.width = renderer2D.texture.width;
                        renderer2D.sourceRec.height = renderer2D.texture.height;
                    }
                    renderer2D.textureLoaded = true;

                    entitiesToSort.push_back(entity);
                }
            }

            void Update(Coordinator &coordinator, AssetManager &assetManager)
            {
                if (entities.size() != entitiesToSort.size()) {
                    entitiesToSort.clear();
                    for (auto const &entity : entities) {
                        auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);
                        auto &transform = coordinator.GetComponent<Transform2D>(entity);

                        if (!renderer2D.textureLoaded) {
                            auto assetOpt = assetManager.getAsset(renderer2D.textureName);
                            if (assetOpt) renderer2D.texture = assetOpt.value()->GetData<Texture2D>();
                            else std::cerr << "Texture " << renderer2D.textureName << " not found" << std::endl;

                            renderer2D.texture.width *= transform.scale.x;
                            renderer2D.texture.height *= transform.scale.y;

                            if (renderer2D.isAnimated) {
                                renderer2D.sourceRec.width = renderer2D.texture.width / renderer2D.nbFrames;
                                renderer2D.sourceRec.height = renderer2D.texture.height;
                                if (renderer2D.animateOnStart) renderer2D.animate = true;
                            } else {
                                renderer2D.sourceRec.width = renderer2D.texture.width;
                                renderer2D.sourceRec.height = renderer2D.texture.height;
                            }

                            renderer2D.textureLoaded = true;
                        }

                        entitiesToSort.push_back(entity);
                    }
                }

                std::sort(entitiesToSort.begin(), entitiesToSort.end(), [&](Entity &e1, Entity &e2) {
                    auto &r1 = coordinator.GetComponent<Renderer2D>(e1);
                    auto &r2 = coordinator.GetComponent<Renderer2D>(e2);
                    return r1.zIndex > r2.zIndex;
                });

                for (auto const &entity : entitiesToSort) {
                    auto &transform = coordinator.GetComponent<Transform2D>(entity);
                    auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

                    if (renderer2D.draw) {
                        if (renderer2D.isAnimated) AnimateSprite(renderer2D, entity, coordinator);
                        DrawTexturePro(renderer2D.texture, renderer2D.sourceRec, {transform.position.x, transform.position.y, (float)renderer2D.texture.width / renderer2D.nbFrames, (float)renderer2D.texture.height}, {0, 0}, transform.rotation, renderer2D.tint);
                    }
                }
            }
        private:
            std::vector<Entity> entitiesToSort;
            void AnimateSprite(Renderer2D &renderer2D, Entity id, Coordinator &coordinator)
            {
                if (!renderer2D.loopAnimation) {
                    if (renderer2D.animate) {
                        renderer2D.currentFrameCounter++;
                        if (renderer2D.currentFrameCounter >= GetFPS() / renderer2D.framesSpeed) {
                            renderer2D.currentFrame++;
                            renderer2D.currentFrameCounter = 0;
                            if (renderer2D.currentFrame >= renderer2D.nbFrames - 1) {
                                renderer2D.animate = false;
                                if (renderer2D.destroyOnAnimationEnd) coordinator.DestroyEntity(id);
                            }
                            renderer2D.sourceRec.x = (float)renderer2D.currentFrame * ((float)renderer2D.texture.width / renderer2D.nbFrames);
                        }
                    }
                } else {
                    renderer2D.currentFrameCounter++;
                    if (renderer2D.currentFrameCounter >= GetFPS() / renderer2D.framesSpeed) {
                        renderer2D.currentFrameCounter = 0;
                        renderer2D.currentFrame++;
                        if (renderer2D.currentFrame >= renderer2D.nbFrames) renderer2D.currentFrame = 0;
                        renderer2D.sourceRec.x = (float)renderer2D.currentFrame * (float)renderer2D.texture.width / renderer2D.nbFrames;
                    }
                }
            }
    };

    // ---------------------------------
    // Physics System
    // ---------------------------------
    class PhysicsSystem : public System {
        public:
            PhysicsSystem() = default;
            ~PhysicsSystem() = default;

            void Start(Coordinator &coordinator)
            {
                for (auto const &entity : entities) {
                    auto &rigidBody = coordinator.GetComponent<RigidBody2D>(entity);

                    rigidBody.force = {0, 0};
                    rigidBody.velocity = {0, 0};
                }
            }

            void Update(Coordinator &coordinator)
            {
                for (auto const &entity : entities) {
                    auto &rigidBody2D = coordinator.GetComponent<RigidBody2D>(entity);
                    auto &transform = coordinator.GetComponent<Transform2D>(entity);
                    auto const &gravity = coordinator.GetComponent<Gravity>(entity);

                    if (gravity.enabled) {
                        rigidBody2D.force.y += rigidBody2D.mass * gravity.force;
                    }
                    transform.position.x += rigidBody2D.velocity.x * GetFrameTime();
                    transform.position.y += rigidBody2D.velocity.y * GetFrameTime();
                    rigidBody2D.velocity.x += (rigidBody2D.force.x / rigidBody2D.mass) * GetFrameTime();
                    rigidBody2D.velocity.y += (rigidBody2D.force.y / rigidBody2D.mass) * GetFrameTime();
                }
            }
    };

    // ---------------------------------
    // Audio System
    // ---------------------------------
    class AudioSystem : public System {
        public:
            AudioSystem() = default;
            ~AudioSystem() = default;

            void Start(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &audio = coordinator.GetComponent<Audio>(entity);

                    if (audio.type == AudioType::SOUND) {
                        auto assetOpt = assetManager.getAsset(audio.audioName);
                        if (assetOpt) audio.sound = assetOpt.value()->GetData<Audio>().sound;
                        else std::cerr << "Sound " << audio.audioName << " not found" << std::endl;
                    }
                    else if (audio.type == AudioType::MUSIC) {
                        auto assetOpt = assetManager.getAsset(audio.audioName);
                        if (assetOpt) audio.music = assetOpt.value()->GetData<Audio>().music;
                        else std::cerr << "Sound " << audio.audioName << " not found" << std::endl;
                    }

                    if (audio.type == AudioType::MUSIC) {
                        if (audio.loopMusic) audio.music.looping = true;
                        if (audio.playOnStart) audio.PlayAudio();
                    }
                    audio.isLoaded = true;
                }
            }

            void Update(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &audio = coordinator.GetComponent<Audio>(entity);

                    if (!audio.isLoaded) {
                        if (audio.type == AudioType::SOUND) {
                            auto assetOpt = assetManager.getAsset(audio.audioName);
                            if (assetOpt) audio.sound = assetOpt.value()->GetData<Audio>().sound;
                            else std::cerr << "Sound " << audio.audioName << " not found" << std::endl;
                        }
                        else if (audio.type == AudioType::MUSIC) {
                            auto assetOpt = assetManager.getAsset(audio.audioName);
                            if (assetOpt) audio.music = assetOpt.value()->GetData<Audio>().music;
                            else std::cerr << "Sound " << audio.audioName << " not found" << std::endl;
                        }

                        if (audio.type == AudioType::MUSIC) {
                            if (audio.loopMusic) audio.music.looping = true;
                            if (audio.playOnStart) audio.PlayAudio();
                        }
                        audio.isLoaded = true;
                    }

                    if (audio.type == AudioType::MUSIC)
                        UpdateMusicStream(audio.music);
                }
            }
    };

    // ---------------------------------
    // UserInterface Systems
    // ---------------------------------
    class TextSystem : public System {
        public:
            TextSystem() = default;
            ~TextSystem() = default;

            void Start(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &text = coordinator.GetComponent<Text>(entity);

                    auto assetOpt = assetManager.getAsset(text.fontName);
                    if (assetOpt) {
                        text.font = assetOpt.value()->GetData<Font>();
                        text.fontLoaded = true;
                    } else {
                        std::cerr << "Font " << text.fontName << " not found" << std::endl;
                    }
                }
            }

            void Update(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &text = coordinator.GetComponent<Text>(entity);
                    if (!text.fontLoaded) {
                        auto assetOpt = assetManager.getAsset(text.fontName);
                        if (assetOpt) {
                            text.font = assetOpt.value()->GetData<Font>();
                            text.fontLoaded = true;
                        } else {
                            std::cerr << "Font " << text.fontName << " not found" << std::endl;
                        }
                    }

                    DrawTextEx(text.font, text.text.c_str(), text.position, text.fontSize, text.spacing, text.color);
                }
            }
    };

    class ButtonSystem : public System {
        public:
            ButtonSystem() = default;
            ~ButtonSystem() = default;

            void Start(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &button = coordinator.GetComponent<Button>(entity);

                    button.rect = {button.position.x, button.position.y, button.width, button.height};

                    auto assetOpt = assetManager.getAsset(button.buttonText.fontName);
                    if (assetOpt) button.buttonText.font = assetOpt.value()->GetData<Font>();
                    else std::cerr << "Font " << button.buttonText.fontName << " not found" << std::endl;

                    button.buttonText.position = {
                        button.position.x + button.width / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).x / 2, 
                        button.position.y + button.height / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).y / 2
                    };
                    button.buttonText.fontLoaded = true;
                    button.set = true;
                }
            }

            void Update(Coordinator &coordinator, AssetManager &assetManager)
            {
                for (auto const &entity : entities) {
                    auto &button = coordinator.GetComponent<Button>(entity);

                    if (!button.set || !button.buttonText.fontLoaded) {
                        button.rect = {button.position.x, button.position.y, button.width, button.height};
                        auto assetOpt = assetManager.getAsset(button.buttonText.fontName);
                        if (assetOpt) button.buttonText.font = assetOpt.value()->GetData<Font>();
                        else std::cerr << "Font " << button.buttonText.fontName << " not found" << std::endl;

                        button.buttonText.position = {
                            button.position.x + button.width / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).x / 2, 
                            button.position.y + button.height / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).y / 2
                        };
                        button.buttonText.fontLoaded = true;
                        button.set = true;
                    }

                    if (button.rounded) {
                        DrawRectangleRounded(button.rect, button.roundness, 0, button.baseColor);
                        DrawTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.position, button.buttonText.fontSize, button.buttonText.spacing, button.buttonText.color);
                    } else {
                        DrawRectangleRec(button.rect, button.baseColor);
                        DrawTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.position, button.buttonText.fontSize, button.buttonText.spacing, button.buttonText.color);
                    }
                }
            }
    };

    // ---------------------------------
    // Collision System
    // ---------------------------------
    class CollisionSystem : public System {
        public:
            CollisionSystem() = default;
            ~CollisionSystem() = default;

            void Start(Coordinator &coordinator)
            {
                for (auto const &entity : entities) {
                    auto &collision = coordinator.GetComponent<Collision2D>(entity);
                    auto &transform = coordinator.GetComponent<Transform2D>(entity);
                    auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

                    if (!collision.colliderSet) {
                        std::cout << "Setting collider for entity " << entity << std::endl;
                        collision.rect.x = transform.position.x;
                        collision.rect.y = transform.position.y;

                        collision.rect.width = renderer2D.sourceRec.width;
                        collision.rect.height = renderer2D.sourceRec.height;

                        collision.colliderSet = true;
                    } else {
                        std::cout << "Collider already set for entity " << entity << std::endl;
                    }
                }
            }

            void Update(Coordinator &coordinator)
            {
                if (entities.size() < 2)
                    return;

                for (auto const &entity : entities) {
                    auto &collision = coordinator.GetComponent<Collision2D>(entity);
                    auto &transform = coordinator.GetComponent<Transform2D>(entity);
                    auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

                    if (!collision.colliderSet) {
                        std::cout << "Collider not set" << std::endl;
                        collision.rect.x = transform.position.x;
                        collision.rect.y = transform.position.y;

                        collision.rect.width = renderer2D.texture.width;
                        collision.rect.height = renderer2D.texture.height;
                        collision.colliderSet = true;
                    }
                }
            }

            void OnGizmoDraw(Coordinator &coordinator)
            {
                for (auto const &entity : entities) {
                    auto &collision = coordinator.GetComponent<Collision2D>(entity);
                    auto &transform = coordinator.GetComponent<Transform2D>(entity);

                    collision.rect.x = transform.position.x;
                    collision.rect.y = transform.position.y;

                    if (collision.debug && collision.enabled) {
                        if (collision.collided)
                            DrawRectangleLinesEx(collision.rect, 1, RED);
                        else
                            DrawRectangleLinesEx(collision.rect, 1, GREEN);
                    }
                }
            }
    };

    // TODO Remove this system after defense
    // ---------------------------------
    // Entity Data System
    // ---------------------------------
    class EntityDataSystem : public System {
        public:
            EntityDataSystem() = default;
            ~EntityDataSystem() = default;

            void Start(Coordinator &coordinator) {}
            void Update(Coordinator &coordinator)
            {
                for (auto const &entity : entities) {
                    auto &entityData = coordinator.GetComponent<EntityData>(entity);

                    if (!entityData.isAlive()) {
                        coordinator.DestroyEntity(entity);
                    }
                }
            }
    };

    //** Input System **//

    class InputSystem {
        public:
            InputSystem() = default;
            ~InputSystem() = default;

            // Static properties
            static Vector2 mousePosition()
            {
                return GetMousePosition();
            }

            static Vector2 mouseScroll()
            {
                return GetMouseWheelMoveV();
            }

            // Static Methods
            static float GetAxis(std::string axisName)
            {
                if (axisName == "Horizontal")
                    return IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
                else if (axisName == "Vertical")
                    return IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
                else {
                    std::cerr << "Axis name not found" << std::endl;
                    return 0;
                }
            }
            static bool GetKey(std::string buttonName)
            {
                std::transform(buttonName.begin(), buttonName.end(), buttonName.begin(), ::toupper);

                if (buttonKeyMap.find(buttonName) != buttonKeyMap.end()) {
                    static std::unordered_map<std::string, float> lastPressTimes;
                    int key = buttonKeyMap[buttonName];

                    double currentTime = GetTime();
                    if (lastPressTimes.find(buttonName) == lastPressTimes.end() ||
                        currentTime - lastPressTimes[buttonName] >= 0.3) {
                        if (IsKeyDown(key)) {
                            lastPressTimes[buttonName] = currentTime;
                            return true;
                        }
                    }
                    return false;
                } else {
                    std::cerr << "Button name not found: " << buttonName << std::endl;
                    return false;
                }
            }

            static bool GetKeyDown(std::string buttonName)
            {
                std::transform(buttonName.begin(), buttonName.end(), buttonName.begin(), ::toupper);

                if (buttonKeyMap.find(buttonName) != buttonKeyMap.end()) {
                    int key = buttonKeyMap[buttonName];
                    return IsKeyDown(key);
                } else {
                    std::cerr << "Button name not found: " << buttonName << std::endl;
                    return false;
                }
            }

            static bool GetKeyUp(std::string buttonName)
            {
                std::transform(buttonName.begin(), buttonName.end(), buttonName.begin(), ::toupper);

                if (buttonKeyMap.find(buttonName) != buttonKeyMap.end()) {
                    int key = buttonKeyMap[buttonName];
                    return IsKeyReleased(key);
                } else {
                    std::cerr << "Button name not found: " << buttonName << std::endl;
                    return false;
                }
            }

            static bool GetMouseButton(int button)
            {
                IsMouseButtonPressed(button);
            }

            static bool GetMouseButtonDown(int button)
            {
                IsMouseButtonDown(button);
            }

            static bool GetMouseButtonUp(int button)
            {
                IsMouseButtonReleased(button);
            }

        private:
            static std::unordered_map<std::string, int> buttonKeyMap;
            static std::unordered_map<std::string, double> lastKeyPressTime;

            static bool CheckCooldown(std::string buttonName, double cooldown)
            {
                double currentTime = GetTime();
                if (lastKeyPressTime.find(buttonName) == lastKeyPressTime.end() ||
                    currentTime - lastKeyPressTime[buttonName] >= cooldown) {
                    lastKeyPressTime[buttonName] = currentTime;
                    return true;
                }
                return false;
            }
    };

    std::unordered_map<std::string, int> InputSystem::buttonKeyMap = {
        {"0", KEY_ZERO},
        {"1", KEY_ONE},
        {"2", KEY_TWO},
        {"3", KEY_THREE},
        {"4", KEY_FOUR},
        {"5", KEY_FIVE},
        {"6", KEY_SIX},
        {"7", KEY_SEVEN},
        {"8", KEY_EIGHT},
        {"9", KEY_NINE},
        {"A", KEY_A},
        {"B", KEY_B},
        {"C", KEY_C},
        {"D", KEY_D},
        {"E", KEY_E},
        {"F", KEY_F},
        {"G", KEY_G},
        {"H", KEY_H},
        {"I", KEY_I},
        {"J", KEY_J},
        {"K", KEY_K},
        {"L", KEY_L},
        {"M", KEY_M},
        {"N", KEY_N},
        {"O", KEY_O},
        {"P", KEY_P},
        {"Q", KEY_Q},
        {"R", KEY_R},
        {"S", KEY_S},
        {"T", KEY_T},
        {"U", KEY_U},
        {"V", KEY_V},
        {"W", KEY_W},
        {"X", KEY_X},
        {"Y", KEY_Y},
        {"Z", KEY_Z},
        {"APOSTROPHE", KEY_APOSTROPHE},
        {"COMMA", KEY_COMMA},
        {"MINUS", KEY_MINUS},
        {"PERIOD", KEY_PERIOD},
        {"SLASH", KEY_SLASH},
        {"SEMICOLON", KEY_SEMICOLON},
        {"EQUAL", KEY_EQUAL},
        {"LEFT_BRACKET", KEY_LEFT_BRACKET},
        {"RIGHT_BRACKET", KEY_RIGHT_BRACKET},
        {"BACKSLASH", KEY_BACKSLASH},
        {"GRAVE", KEY_GRAVE},
        {"SPACE", KEY_SPACE},
        {"ESPACE", KEY_ESCAPE},
        {"ENTER", KEY_ENTER},
        {"TAB", KEY_TAB},
        {"BACKSPACE", KEY_BACKSPACE},
        {"INSERT", KEY_INSERT},
        {"DELETE", KEY_DELETE},
        {"RIGHT", KEY_RIGHT},
        {"LEFT", KEY_LEFT},
        {"DOWN", KEY_DOWN},
        {"UP", KEY_UP},
        {"PAGEUP", KEY_PAGE_UP},
        {"PAGEDOWN", KEY_PAGE_DOWN},
        {"HOME", KEY_HOME},
        {"END", KEY_END},
        {"CAPSLOCK", KEY_CAPS_LOCK},
        {"SCROLLLOCK", KEY_SCROLL_LOCK},
        {"NUMLOCK", KEY_NUM_LOCK},
        {"PRINTSCREEN", KEY_PRINT_SCREEN},
        {"PAUSE", KEY_PAUSE},
        {"F1", KEY_F1},
        {"F2", KEY_F2},
        {"F3", KEY_F3},
        {"F4", KEY_F4},
        {"F5", KEY_F5},
        {"F6", KEY_F6},
        {"F7", KEY_F7},
        {"F8", KEY_F8},
        {"F9", KEY_F9},
        {"F10", KEY_F10},
        {"F11", KEY_F11},
        {"F12", KEY_F12},
        {"LEFTSHIFT", KEY_LEFT_SHIFT},
        {"LEFTCONTROL", KEY_LEFT_CONTROL},
        {"LEFTALT", KEY_LEFT_ALT},
        {"RIGHTSHIFT", KEY_RIGHT_SHIFT},
        {"RIGHTCONTROL", KEY_RIGHT_CONTROL},
        {"RIGHTALT", KEY_RIGHT_ALT},
        {"MOUSELEFT", MOUSE_LEFT_BUTTON},
        {"MOUSERIGHT", MOUSE_RIGHT_BUTTON},
        {"MOUSEMIDDLE", MOUSE_MIDDLE_BUTTON},
    };

    std::unordered_map<std::string, double> InputSystem::lastKeyPressTime = {
        {"0", 0.0f},
        {"1", 0.0f},
        {"2", 0.0f},
        {"3", 0.0f},
        {"4", 0.0f},
        {"5", 0.0f},
        {"6", 0.0f},
        {"7", 0.0f},
        {"8", 0.0f},
        {"9", 0.0f},
        {"A", 0.0f},
        {"B", 0.0f},
        {"C", 0.0f},
        {"D", 0.0f},
        {"E", 0.0f},
        {"F", 0.0f},
        {"G", 0.0f},
        {"H", 0.0f},
        {"I", 0.0f},
        {"J", 0.0f},
        {"K", 0.0f},
        {"L", 0.0f},
        {"M", 0.0f},
        {"N", 0.0f},
        {"O", 0.0f},
        {"P", 0.0f},
        {"Q", 0.0f},
        {"R", 0.0f},
        {"S", 0.0f},
        {"T", 0.0f},
        {"U", 0.0f},
        {"V", 0.0f},
        {"W", 0.0f},
        {"X", 0.0f},
        {"Y", 0.0f},
        {"Z", 0.0f},
        {"APOSTROPHE", 0.0f},
        {"COMMA", 0.0f},
        {"MINUS", 0.0f},
        {"PERIOD", 0.0f},
        {"SLASH", 0.0f},
        {"SEMICOLON", 0.0f},
        {"EQUAL", 0.0f},
        {"LEFT_BRACKET", 0.0f},
        {"RIGHT_BRACKET", 0.0f},
        {"BACKSLASH", 0.0f},
        {"GRAVE", 0.0f},
        {"SPACE", 0.0f},
        {"ESPACE", 0.0f},
        {"ENTER", 0.0f},
        {"TAB", 0.0f},
        {"BACKSPACE", 0.0f},
        {"INSERT", 0.0f},
        {"DELETE", 0.0f},
        {"RIGHT", 0.0f},
        {"LEFT", 0.0f},
        {"DOWN", 0.0f},
        {"UP", 0.0f},
        {"PAGEUP", 0.0f},
        {"PAGEDOWN", 0.0f},
        {"HOME", 0.0f},
        {"END", 0.0f},
        {"CAPSLOCK", 0.0f},
        {"SCROLLLOCK", 0.0f},
        {"NUMLOCK", 0.0f},
        {"PRINTSCREEN", 0.0f},
        {"PAUSE", 0.0f},
        {"F1", 0.0f},
        {"F2", 0.0f},
        {"F3", 0.0f},
        {"F4", 0.0f},
        {"F5", 0.0f},
        {"F6", 0.0f},
        {"F7", 0.0f},
        {"F8", 0.0f},
        {"F9", 0.0f},
        {"F10", 0.0f},
        {"F11", 0.0f},
        {"F12", 0.0f},
        {"LEFTSHIFT", 0.0f},
        {"LEFTCONTROL", 0.0f},
        {"LEFTALT", 0.0f},
        {"RIGHTSHIFT", 0.0f},
        {"RIGHTCONTROL", 0.0f},
        {"RIGHTALT", 0.0f},
    };


    //** Engine **//
    enum Mode {
        Mode2D,
        Mode3D,
    };

    class Engine {
        public:
            Engine(std::string gameName, int width, int height, Mode mode, const std::string& assetsDirectory, bool render = true) : _render(render), _assetManager(AssetManager::GetInstance()), _mode(mode), _camera(mode == Mode2D ? CameraType::CAMERA_2D : CameraType::CAMERA_3D)
            {
                _isRunning = true;
                if (_render) {
                    InitWindow(width, height, gameName.c_str());
                    InitAudioDevice();
                    SetTargetFPS(60);
                    _assetManager.bulkLoadAssets(assetsDirectory);
                }
            }

            ~Engine()
            {
                if (_render) {
                    // _assetManager.unloadAllAssets();
                    CloseAudioDevice();
                    CloseWindow();
                }
            }

            void Init()
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
    
            void SetAssetsDirectory(const std::string& assetsDirectory)
            {
                _assetsDirectory = assetsDirectory;
            }

            void Run()
            {
                while (_render ? !WindowShouldClose() && _isRunning : _isRunning)
                {
                    Update();
                    if (_render) Render();
                    LateUpdate();
                }
            }

            void Stop()
            {
                _isRunning = false;
            }

            void Start()
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

            void Update()
            {
                if (_render) _audio->Update(_coordinator, _assetManager);
                _physics->Update(_coordinator);
                _collision->Update(_coordinator);
                _entityData->Update(_coordinator);
                _sceneManager.Update();
            }

            void Render()
            {
                _mode ? Render3D() : Render2D();
            }

            void LateUpdate()
            {
                _sceneManager.LateUpdate();
            }

            unsigned int AddScene(std::shared_ptr<Scene> scene)
            {
                return _sceneManager.AddScene(scene);
            }

            void SwitchScene(unsigned int id)
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

            void RemoveScene(unsigned int id)
            {
                _sceneManager.RemoveScene(id);
            }

            // Camera
            void SetCamera(Camera camera)
            {
                if ((camera.Type() == CameraType::CAMERA_2D && _mode == Mode3D) ||
                    (camera.Type() == CameraType::CAMERA_3D && _mode == Mode2D))
                    return;
                _camera = camera;
            }

            Camera& GetCamera()
            {
                return _camera;
            }

            // ECS functions
            Entity CreateEntity()
            {
                return _coordinator.CreateEntity();
            }

            Entity CreateCustomEntity(int id)
            {
                return _coordinator.CreateCustomEntity(id);
            }

            void DestroyEntity(Entity entity)
            {
                _coordinator.DestroyEntity(entity);
                std::cout << "Destroyed entity " << entity << std::endl;
            }

            bool EntityExists(Entity entity)
            {
                return _coordinator.EntityExists(entity);
            }

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
            Vector2 GetScreenSize() const
            {
                return {static_cast<float>(GetScreenWidth()), static_cast<float>(GetScreenHeight())};
            }

            void SetAudioVolume(float volume)
            {
                SetMasterVolume(volume);
            }

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

            void Render2D()
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

            void Render3D()
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
    };
}

#endif /* !RAYLIB_H_ */