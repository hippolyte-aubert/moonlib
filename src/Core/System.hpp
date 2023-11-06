/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** System
*/

#ifndef SYSTEM_HPP_
    #define SYSTEM_HPP_

    #include <iostream>
    #include <set>
    #include <memory>
    #include <unordered_map>
    #include "Entity.hpp"

namespace ECS {
    class System {
        public:
            void Clear()
            {
                entities.clear();
            }
            std::set<Entity> entities;
    };

    class SystemManager
    {
        public:
            template<typename T>
            std::shared_ptr<T> RegisterSystem()
            {
                std::string typeName = typeid(T).name();

                assert(_systems.find(typeName) == _systems.end() && "Registering system more than once.");

                auto system = std::make_shared<T>();
                _systems.insert({typeName, system});
                return system;
            }

            template<typename T>
            void SetSignature(Signature signature)
            {
                std::string typeName = typeid(T).name();
                std::cout << "Setting signature for " << typeName << std::endl;

                assert(_systems.find(typeName) != _systems.end() && "System used before registered.");

                _signatures.insert({typeName, signature});
            }

            void EntityDestroyed(Entity entity)
            {
                for (auto const& pair : _systems)
                {
                    auto const& system = pair.second;

                    system->entities.erase(entity);
                }
            }

            void EntitySignatureChanged(Entity entity, Signature entitySignature)
            {
                for (auto const& pair : _systems)
                {
                    auto const& type = pair.first;
                    auto const& system = pair.second;
                    auto const& systemSignature = _signatures[type];

                    if ((entitySignature & systemSignature) == systemSignature)
                    {
                        system->entities.insert(entity);
                    }
                    else
                    {
                        system->entities.erase(entity);
                    }
                }
            }

        private:
            std::unordered_map<std::string, Signature> _signatures{};
            std::unordered_map<std::string, std::shared_ptr<System>> _systems{};
    };
}

#endif /* !SYSTEM_HPP_ */
