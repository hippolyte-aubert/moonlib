/*
** EPITECH PROJECT, 2023
** Engine
** File description:
** Component
*/

#ifndef COMPONENT_HPP_
    #define COMPONENT_HPP_

    #include <cstdint>
    #include <cassert>
    #include <memory>
    #include <unordered_map>
    #include "Entity.hpp"

namespace ECS {
    using ComponentType = std::uint8_t;

    class IComponentArray {
        public:
        virtual ~IComponentArray() = default;
        virtual void EntityDestroyed(Entity entity) = 0;
    };

    template<typename T>
    class ComponentArray : public IComponentArray {
        public:
        void InsertData(Entity entity, T component)
        {
            assert(mEntityToIndexMap.find(entity) == mEntityToIndexMap.end() && "Component already added to this entity.");

            size_t newIndex = mSize;
            mEntityToIndexMap[entity] = newIndex;
            mIndexToEntityMap[newIndex] = entity;
            mComponentArray[newIndex] = component;
            ++mSize;
        }

        void RemoveData(Entity entity)
        {
            assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Component does not exist in this entity");
            
            size_t indexOfRemovedEntity = mEntityToIndexMap[entity];
            size_t indexOfLastElement = mSize - 1;
            mComponentArray[indexOfRemovedEntity] = mComponentArray[indexOfLastElement];

            Entity entityOfLastElement = mIndexToEntityMap[indexOfLastElement];
            mEntityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
            mIndexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

            mEntityToIndexMap.erase(entity);
            mIndexToEntityMap.erase(indexOfLastElement);

            --mSize;
        }

        T& GetData(Entity entity)
        {
            //assert(mEntityToIndexMap.find(entity) != mEntityToIndexMap.end() && "Retrieving non-existent component.");

            return mComponentArray[mEntityToIndexMap[entity]];
        }

        void EntityDestroyed(Entity entity) override
        {
            if (mEntityToIndexMap.find(entity) != mEntityToIndexMap.end())
            {
                RemoveData(entity);
            }
        }
        private:
            std::array<T, MAX_ENTITIES> mComponentArray;
            std::unordered_map<Entity, size_t> mEntityToIndexMap;
            std::unordered_map<size_t, Entity> mIndexToEntityMap;
            size_t mSize;
    };

    class ComponentManager
    {
        public:
            ~ComponentManager() = default;
        
            template<typename T>
            void RegisterComponent()
            {
                std::string typeName = typeid(T).name();

                std::cout << "Registering component type: " << typeName << std::endl;

                assert(_componentTypes.find(typeName) == _componentTypes.end() && "Registering component type more than once.");

                _componentTypes.insert({typeName, _nextComponentType});
                _componentArrays.insert({typeName, std::make_shared<ComponentArray<T>>()});
                ++_nextComponentType;
            }

            template<typename T>
            ComponentType GetComponentType()
            {
                std::string typeName = typeid(T).name();

                assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

                return _componentTypes[typeName];
            }

            template<typename T>
            void AddComponent(Entity entity, T component)
            {
                std::string typeName = typeid(T).name();
                GetComponentArray<T>()->InsertData(entity, component);
            }

            template<typename T>
            void RemoveComponent(Entity entity)
            {
                GetComponentArray<T>()->RemoveData(entity);
            }

            template<typename T>
            T& GetComponent(Entity entity)
            {
                return GetComponentArray<T>()->GetData(entity);
            }

            void EntityDestroyed(Entity entity)
            {
                for (auto const& pair : _componentArrays)
                {
                    auto const& component = pair.second;

                    component->EntityDestroyed(entity);
                }
            }

        private:
            std::unordered_map<std::string , ComponentType> _componentTypes;
            std::unordered_map<std::string , std::shared_ptr<IComponentArray>> _componentArrays;
            ComponentType _nextComponentType;

            template<typename T>
            std::shared_ptr<ComponentArray<T>> GetComponentArray()
            {
                std::string typeName = typeid(T).name();

                assert(_componentTypes.find(typeName) != _componentTypes.end() && "Component not registered before use.");

                return std::static_pointer_cast<ComponentArray<T>>(_componentArrays[typeName]);
            }
    };
}

#endif /* !COMPONENT_HPP_ */
