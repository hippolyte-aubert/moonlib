/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** UserInterfaceSystem
*/

#ifndef USERINTERFACESYSTEM_HPP_
    #define USERINTERFACESYSTEM_HPP_

    #include "../Core/System.hpp"
    #include "../Core/Coordinator.hpp"
    #include "../Components/UserInterface.hpp"
    #include "../Core/Asset.hpp"

namespace ECS {
    class TextSystem : public System {
        public:
            TextSystem() = default;
            ~TextSystem() = default;

            void Start(Coordinator &coordinator, AssetManager &assetManager);
            void Update(Coordinator &coordinator, AssetManager &assetManager);

        protected:
        private:
    };

    class ButtonSystem : public System {
        public:
            ButtonSystem() = default;
            ~ButtonSystem() = default;

            void Start(Coordinator &coordinator, AssetManager &assetManager);
            void Update(Coordinator &coordinator, AssetManager &assetManager);

        protected:
        private:
    };
}

// class InputTextSystem : public System {
//     public:
//         InputTextSystem() = default;
//         ~InputTextSystem() = default;

//         void Start(Coordinator &coordinator);
//         void Update(Coordinator &coordinator);

//     protected:
//     private:
// };

#endif /* !USERINTERFACESYSTEM_HPP_ */
