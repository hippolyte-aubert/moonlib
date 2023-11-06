/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Entity
*/

#ifndef ENTITY_HPP_
    #define ENTITY_HPP_

    #include <bitset>
    #include <iostream>
    #include <queue>
    #include <array>
    #include <cstdint>
    #include <cassert>

    #define MAX_COMPONENTS 128
    #define MAX_ENTITIES 100
    #define MAX_SIZE_TAB 100001

namespace ECS {
    using Signature = std::bitset<MAX_COMPONENTS>;
    using Entity = std::uint32_t;

    class EntityManager {
        public:
            EntityManager()
            {
                _livingEntityCount = 0;

                for (std::uint32_t id = 0; id < MAX_ENTITIES; ++id) {
                    std::uint32_t id_rng = rand() % 10000;
                    Entity entity(id_rng);
                    _availableEntities.push(entity);
                }
            }

            Entity CreateCustomEntity(int id)
            {
                uint32_t id_casted = static_cast<uint32_t>(id);

                Entity entity(id_casted);
                std::cout << "Entity " << entity << " created" << std::endl;
                ++_livingEntityCount;
                return entity;
            }

            Entity CreateEntity()
            {
                assert(_livingEntityCount < MAX_ENTITIES && "Too many entities in existence.");

                Entity id = _availableEntities.front();
                std::cout << "Entity " << id << " created" << std::endl;
                _availableEntities.pop();
                ++_livingEntityCount;
                return id;
            }

            void DestroyEntity(Entity entity)
            {
                _signatures[entity].reset();
                _availableEntities.push(entity);
                --_livingEntityCount;
            }

            void SetSignature(Entity entity, Signature signature)
            {
                _signatures[entity] = signature;
            }

            Signature GetSignature(Entity entity)
            {
                return _signatures[entity];
            }

            std::queue<Entity> GetAvailableEntities() const
            {
                return _availableEntities;
            }

            bool EntityExists(Entity entity)
            {
                return _signatures[entity].any();
            }

        private:
            std::queue<Entity> _availableEntities;
            std::array<Signature, MAX_SIZE_TAB> _signatures;
            std::uint32_t _livingEntityCount;
    };
}

#endif /* !ENTITY_HPP_ */
