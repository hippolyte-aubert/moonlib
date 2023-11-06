/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** RenderingSystem
*/

#ifndef RENDERINGSYSTEM_HPP_
    #define RENDERINGSYSTEM_HPP_

    #include "../Core/System.hpp"
    #include "../Components/Rendering.hpp"
    #include "../Components/Physics.hpp"
    #include "../Core/Coordinator.hpp"
    #include "../Core/Asset.hpp"
    #include <vector>
    #include <algorithm>

namespace ECS {
    class RenderingSystem : public System {
        public:
            RenderingSystem();
            ~RenderingSystem();

            void Start(Coordinator &coordinator, AssetManager &assetManager);
            void Update(Coordinator &coordinator, AssetManager &assetManager);

        protected:
        private:
            std::vector<Entity> entitiesToSort;
            void AnimateSprite(Renderer2D &renderer2D, Entity id, Coordinator &coordinator);
    };
}

#endif /* !RENDERINGSYSTEM_HPP_ */
