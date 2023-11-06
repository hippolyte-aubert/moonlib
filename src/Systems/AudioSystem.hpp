/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** AudioSystem
*/

#ifndef AUDIOSYSTEM_HPP_
    #define AUDIOSYSTEM_HPP_

    #include "../Core/System.hpp"
    #include "../Components/Audio.hpp"
    #include "../Core/Coordinator.hpp"
    #include "../Core/Asset.hpp"

namespace ECS {
    class AudioSystem : public System {
        public:
            AudioSystem();
            ~AudioSystem();

            void Start(Coordinator &coordinator, AssetManager &assetManager);
            void Update(Coordinator &coordinator, AssetManager &assetManager);
        protected:
        private:
    };
}

#endif /* !AUDIOSYSTEM_HPP_ */
