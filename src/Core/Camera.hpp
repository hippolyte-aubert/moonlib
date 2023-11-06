/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Camera
*/

#ifndef CAMERA_HPP_
    #define CAMERA_HPP_

    #include <raylib.h>

namespace ECS {
    enum ProjectionType {
        PERSPECTIVE = 0,
        ORTHOGRAPHIC = 1
    };

    enum CameraType {
        CAMERA_2D = 0,
        CAMERA_3D = 1
    };

    enum CameraMode {
        CUSTOM = 0,
        FREE = 1,
        ORBITAL = 2,
        FIRST_PERSON = 3,
        THIRD_PERSON = 4
    };
    class Camera {
        public:
            Camera(CameraType type);
            Camera(Vector2 target, Vector2 offset, float rotation, float zoom);
            Camera(Vector3 position, Vector3 target, Vector3 up, float fovy, ProjectionType projection, CameraMode cameraMode);
            Camera(Camera2D camera);
            Camera(Camera3D camera);
            ~Camera();

            Camera2D GetCamera2D() {return _camera2D;};
            Camera3D GetCamera3D() {return _camera3D;};

            void Type(CameraType type) {_type = type;};
            CameraType Type() {return _type;};

            void Mode(CameraMode mode) {_mode = mode;};
            CameraMode Mode() {return _mode;};

            void UpdateCamera() {_updateCamera = true;};
            bool IsCameraUpdating() {return _updateCamera;};

        private:
            Camera2D _camera2D;
            Camera3D _camera3D;
            CameraType _type;
            CameraMode _mode;
            bool _updateCamera = true;
    };
}

#endif /* !CAMERA_HPP_ */
