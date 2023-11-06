/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Asset
*/

#include "Asset.hpp"

namespace ECS {

    AssetManager* AssetManager::_instance = nullptr;

    template<typename T>
    void AssetManager::loadAsset(std::string name, std::string path, AssetType type)
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

    void AssetManager::unloadAsset(std::string name)
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

    std::optional<Asset*> AssetManager::getAsset(const std::string& name)
    {
        if (assets.find(name) != assets.end()) {
            std::cout << "Asset " << name << " found" << std::endl;
            return &assets[name];
        }
        return std::nullopt;
    }

    std::unordered_map<std::string, Asset>& AssetManager::getAssets()
    {
        return assets;
    }

    // void AssetManager::cacheAsset(std::string name, std::string path,AssetType type)
    // {
    //     if (cache.find(name) != cache.end()) {
    //         Asset& asset = assets[name];
    //         cache[name] = asset;
    //         cacheUsageOrder.push_front(name);
    //         asset.refCount++;
    //     } else {
    //         Asset asset;
    //         asset.name = name;
    //         asset.type = type;
    //         asset.refCount = 1;
    //         cache[name] = asset;
    //         cacheUsageOrder.push_back(name);
    //     }
    // }

    // void AssetManager::uncacheAsset(std::string name)
    // {
    //     if (cache.find(name) != cache.end()) {
    //         Asset& asset = cache[name];
    //         asset.refCount--;
    //         if (asset.refCount <= 0) {
    //             cache.erase(name);
    //             cacheUsageOrder.remove(name);
    //         }
    //     } else
    //         std::cerr << "Asset " << name << " not found" << std::endl;
    // }

    // Asset* AssetManager::getCachedAsset(std::string name)
    // {
    //     if (cache.find(name) != cache.end()) {
    //         cacheUsageOrder.remove(name);
    //         cacheUsageOrder.push_front(name);
    //         return &cache[name];
    //     }
    //     return nullptr;
    // }

    // void AssetManager::clearCache()
    // {
    //     cache.clear();
    //     cacheUsageOrder.clear();
    // }

    void AssetManager::clearAssets()
    {
        assets.clear();
    }

    void AssetManager::bulkLoadAssets(const std::string& assetsDirectory)
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

    void AssetManager::unloadAllAssets()
    {
        for (auto& asset : assets)
            unloadAsset(asset.first);
        assets.clear();
    }

    AssetType AssetManager::getAssetExtension(const std::string& assetPath)
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
}