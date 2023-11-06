/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** PhysicsSystem
*/

#include "PhysicsSystem.hpp"
#include "../Core/Coordinator.hpp"

namespace ECS {
    PhysicsSystem::PhysicsSystem()
    {
    }

    PhysicsSystem::~PhysicsSystem()
    {
    }

    void PhysicsSystem::Start(Coordinator &coordinator)
    {
        for (auto const &entity : entities) {
            auto &rigidBody = coordinator.GetComponent<RigidBody2D>(entity);

            rigidBody.force = {0, 0};
            rigidBody.velocity = {0, 0};
        }
    }

    void PhysicsSystem::Update(Coordinator &coordinator)
    {
        for (auto const &entity : entities) {
            auto &rigidBody2D = coordinator.GetComponent<RigidBody2D>(entity);
            auto &transform = coordinator.GetComponent<Transform2D>(entity);
            auto const &gravity = coordinator.GetComponent<Gravity>(entity);

            if (gravity.enabled) {
                rigidBody2D.force.y += rigidBody2D.mass * gravity.force;
            }
            transform.position.x += rigidBody2D.velocity.x * GetFrameTime();
            transform.position.y += rigidBody2D.velocity.y * GetFrameTime();
            rigidBody2D.velocity.x += (rigidBody2D.force.x / rigidBody2D.mass) * GetFrameTime();
            rigidBody2D.velocity.y += (rigidBody2D.force.y / rigidBody2D.mass) * GetFrameTime();

        }
    }
}
