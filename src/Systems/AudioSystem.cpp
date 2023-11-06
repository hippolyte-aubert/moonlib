/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** AudioSystem
*/

#include "AudioSystem.hpp"

namespace ECS {
    AudioSystem::AudioSystem()
    {
    }

    AudioSystem::~AudioSystem()
    {
    }

    void AudioSystem::Start(Coordinator& coordinator, AssetManager& assetManager)
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

    void AudioSystem::Update(Coordinator& coordinator, AssetManager& assetManager)
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
}