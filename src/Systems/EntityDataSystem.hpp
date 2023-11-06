/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** EntityDataSystem
*/

#ifndef ENTITYDATASYSTEM_HPP_
#define ENTITYDATASYSTEM_HPP_

#include "../Core/System.hpp"
#include "../Core/Coordinator.hpp"
#include "../Components/EntityData.hpp"
#include "../Components/Rendering.hpp"

namespace ECS {
    class EntityDataSystem : public System{
        public:
            EntityDataSystem();
            ~EntityDataSystem();

            void Start(Coordinator &coordinator);
            void Update(Coordinator &coordinator);

        protected:
        private:
    };
}

#endif /* !ENTITYDATASYSTEM_HPP_ */
