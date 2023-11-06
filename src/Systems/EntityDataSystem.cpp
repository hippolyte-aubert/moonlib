/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** EntityDataSystem
*/

#include "EntityDataSystem.hpp"

namespace ECS {
    EntityDataSystem::EntityDataSystem()
    {
    }

    EntityDataSystem::~EntityDataSystem()
    {
    }

    void EntityDataSystem::Start(Coordinator& coordinator)
    {
    }

    void EntityDataSystem::Update(Coordinator& coordinator)
    {
        for (auto const &entity : entities) {
            auto &entityData = coordinator.GetComponent<EntityData>(entity);
            auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

            if (!entityData.isAlive())
                renderer2D.draw = false;
        }
    }
}