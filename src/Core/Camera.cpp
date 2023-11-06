/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Camera
*/

#include "Camera.hpp"

namespace ECS {
    Camera::Camera(CameraType type)
    {
        if (type == CAMERA_2D) {
            _camera2D = {
                .offset = {0, 0},
                .target = {0, 0},
                .rotation = 0,
                .zoom = 1.0f,
            };
            _updateCamera = true;
            _camera3D = {0};
        }

        if (type == CAMERA_3D) {
            _camera3D = {
                .position = {0, 0, 0},
                .target = {0, 0, 0},
                .up = {0, 1, 0},
                .fovy = 70,
                .projection = PERSPECTIVE,
            
            };
            _updateCamera = true;
            _camera2D = {0};
        }

        _type = type;
    }

    Camera::Camera(Vector2 target, Vector2 offset, float rotation, float zoom)
    {
        _camera2D = {0};
        _camera2D.target = target;
        _camera2D.offset = offset;
        _camera2D.rotation = rotation;
        _camera2D.zoom = zoom;
        _type = CAMERA_2D;
    }

    Camera::Camera(Vector3 position, Vector3 target, Vector3 up, float fovy, ProjectionType projection, CameraMode cameraMode)
    {
        _camera3D = {0};
        _camera3D.position = position;
        _camera3D.target = target;
        _camera3D.up = up;
        _camera3D.fovy = fovy;
        _camera3D.projection = projection;
        _type = CAMERA_3D;
        _mode = cameraMode;
    }

    Camera::Camera(Camera2D camera)
    {
        _camera2D = camera;
    }

    Camera::Camera(Camera3D camera)
    {
        _camera3D = camera;
    }

    Camera::~Camera()
    {
    }
}