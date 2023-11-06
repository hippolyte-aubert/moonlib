/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** CollisionSystem
*/

#include "CollisionSystem.hpp"

namespace ECS {
    CollisionSystem::CollisionSystem()
    {
    }

    CollisionSystem::~CollisionSystem()
    {
    }

    void CollisionSystem::Start(Coordinator &coordinator)
    {
        for (auto const &entity : entities) {
            auto &collision = coordinator.GetComponent<Collision2D>(entity);
            auto &transform = coordinator.GetComponent<Transform2D>(entity);
            auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

            if (!collision.colliderSet) {
                std::cout << "Setting collider for entity " << entity << std::endl;
                collision.rect.x = transform.position.x;
                collision.rect.y = transform.position.y;

                collision.rect.width = renderer2D.sourceRec.width;
                collision.rect.height = renderer2D.sourceRec.height;

                collision.colliderSet = true;
            } else {
                std::cout << "Collider already set for entity " << entity << std::endl;
            }
        }
    }

    void CollisionSystem::Update(Coordinator &coordinator)
    {
        if (entities.size() < 2)
            return;

        for (auto const &entity : entities) {
            auto &collision = coordinator.GetComponent<Collision2D>(entity);
            auto &transform = coordinator.GetComponent<Transform2D>(entity);
            auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

            if (!collision.colliderSet) {
                std::cout << "Collider not set" << std::endl;
                collision.rect.x = transform.position.x;
                collision.rect.y = transform.position.y;

                collision.rect.width = renderer2D.texture.width;
                collision.rect.height = renderer2D.texture.height;
                collision.colliderSet = true;
            }
        }
    }

    void CollisionSystem::OnGizmoDraw(Coordinator &coordinator)
    {
        for (auto const &entity : entities) {
            auto &collision = coordinator.GetComponent<Collision2D>(entity);
            auto &transform = coordinator.GetComponent<Transform2D>(entity);

            collision.rect.x = transform.position.x;
            collision.rect.y = transform.position.y;

            if (collision.debug && collision.enabled) {
                if (collision.collided)
                    DrawRectangleLinesEx(collision.rect, 1, RED);
                else
                    DrawRectangleLinesEx(collision.rect, 1, GREEN);
            }
        }
    }
}