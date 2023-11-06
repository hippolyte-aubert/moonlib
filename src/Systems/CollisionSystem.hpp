/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** CollisionSystem
*/

#ifndef COLLISIONSYSTEM_HPP_
    #define COLLISIONSYSTEM_HPP_

    #include "../Core/System.hpp"
    #include "../Components/Collision.hpp"
    #include "../Components/Physics.hpp"
    #include "../Components/Rendering.hpp"
    #include "../Core/Coordinator.hpp"
    #include <raymath.h>

namespace ECS {
    class CollisionSystem : public System {
        public:
            CollisionSystem();
            ~CollisionSystem();

            void Start(Coordinator &coordinator);
            void Update(Coordinator &coordinator);
            void OnGizmoDraw(Coordinator &coordinator);

        protected:
        private:
    };
}

#endif /* !COLLISIONSYSTEM_HPP_ */
