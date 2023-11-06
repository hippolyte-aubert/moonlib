/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Collision
*/

#ifndef COLLISION_HPP_
    #define COLLISION_HPP_

    #include <iostream>
    #include <raylib.h>
    #include <set>
    #include <cmath>
    #include "../Core/Coordinator.hpp"

namespace ECS {
    struct Collision2D {
        bool isTrigger = false;
        Rectangle rect = {0, 0, 0, 0};
        bool enabled = true;
        bool debug = false;
        std::string tag = "default";
        bool collided = false;
        bool trigger = false;
        bool colliderSet = false;

        void Solve(std::set<Entity> entities, Coordinator &coordinator, Entity self)
        {
            for (auto const &entity : entities)
            {
                auto &other = coordinator.GetComponent<Collision2D>(entity);

                if (entity == self)
                    continue;
                float xDistance = std::pow((rect.x + rect.width /2 ) - (other.rect.x + other.rect.width /2) , 2);
                float yDistance = std::pow((rect.y + rect.height /2 ) - (other.rect.y + other.rect.height /2) , 2);

                float distance = std::sqrt(xDistance + yDistance);
                // std::cout << "Distance with entity " << entity << ": " << distance << std::endl << std::flush;
                
                if (IsColliding(other))
                {
                    // collided = true;
                    // std::cout << "Entity " << self << " colliding with entity " << entity << std::endl;
                    // TODO : Add collision resolution
                }
            }
        }

        bool IsColliding(Collision2D other)
        {
            if (CheckCollisionRecs(rect, other.rect) && enabled && other.enabled)
                return true;
            return false;
        }

        bool IsTriggering(Collision2D other)
        {
            if (CheckCollisionRecs(rect, other.rect) && (isTrigger || other.isTrigger))
                return true;
            return false;
        }
    };
}

#endif /* !COLLISION_HPP_ */
