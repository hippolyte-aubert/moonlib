/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Physics
*/

#ifndef PHYSICS_HPP_
    #define PHYSICS_HPP_

    #include <raylib.h>

namespace ECS {

    struct Transform2D {
        Vector2 position;
        float rotation;
        Vector2 scale;
    };

    struct Gravity {
        float force;
        bool enabled;
    };

    struct RigidBody {
        Vector3 velocity;
        Vector3 force;
        float mass;
    };

    struct RigidBody2D {
        Vector2 velocity;
        Vector2 force;
        float mass;
    };
}

#endif /* !PHYSICS_HPP_ */
