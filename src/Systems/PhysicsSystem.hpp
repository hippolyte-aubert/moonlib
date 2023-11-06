/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** PhysicsSystem
*/

#ifndef PHYSICSSYSTEM_HPP_
    #define PHYSICSSYSTEM_HPP_

    #include "../Core/System.hpp"
    #include "../Components/Physics.hpp"
    #include "../Core/Coordinator.hpp"

namespace ECS {
    class PhysicsSystem : public System {
        public:
            PhysicsSystem();
            ~PhysicsSystem();

            void Start(Coordinator &coordinator);
            void Update(Coordinator &coordinator);

        protected:
        private:
    };
}

#endif /* !PHYSICSSYSTEM_HPP_ */
