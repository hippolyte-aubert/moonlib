/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Asset
*/

#ifndef ASSET_HPP_
    #define ASSET_HPP_

    #include <iostream>
    #include <unordered_map>
    #include <list>
    #include <filesystem>
    #include <raylib.h>
    #include <any>
    #include <optional>
    #include "../Components/Audio.hpp"

    #define CACHE_SIZE_LIMIT 100

namespace ECS {

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

    class AssetManager {
        public:
            static AssetManager& GetInstance() {
                if (_instance == nullptr)
                    _instance = new AssetManager();
                return *_instance;
            }

            template<typename T>
            void loadAsset(std::string name, std::string path,AssetType type);
            void unloadAsset(std::string name);

            std::optional<Asset*> getAsset(const std::string& name);
            std::unordered_map<std::string, Asset>& getAssets();

            // void cacheAsset(std::string name, std::string path,AssetType type);
            // void uncacheAsset(std::string name);
            // Asset* getCachedAsset(std::string name);

            // void clearCache();
            void clearAssets();
            void bulkLoadAssets(const std::string& assetsDirectory);
            void unloadAllAssets();

            AssetType getAssetExtension(const std::string& assetPath);
        private:
            static AssetManager* _instance;
            std::unordered_map<std::string, Asset> assets;
            // std::unordered_map<std::string, Asset> cache;
            // std::list<std::string> cacheUsageOrder;

            AssetManager() {}
            AssetManager(AssetManager const &) = delete;
            AssetManager& operator=(AssetManager const &) = delete;
    };

    Audio LoadAudio(const std::string& path);
}

#endif /* !ASSET_HPP_ */
