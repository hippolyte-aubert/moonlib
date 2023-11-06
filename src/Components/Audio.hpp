/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Audio
*/

#ifndef AUDIO_HPP_
    #define AUDIO_HPP_

    #include <raylib.h>
    #include <iostream>

namespace ECS {

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
}

#endif /* !AUDIO_HPP_ */
