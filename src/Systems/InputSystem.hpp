/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** InputSystems
*/

#ifndef INPUTSYSTEM_HPP_
    #define INPUTSYSTEM_HPP_

    #include "../Core/System.hpp"
    #include "../Core/Coordinator.hpp"
    #include <algorithm>
    #include <raylib.h>

namespace ECS {
    class InputSystem {
        public:
            InputSystem() = default;
            ~InputSystem() = default;

            // Static properties
            static Vector2 mousePosition();
            static Vector2 mouseScroll();

            // Static Methods
            static float GetAxis(std::string axisName);
            static bool GetKey(std::string buttonName);
            static bool GetKeyDown(std::string buttonName);
            static bool GetKeyUp(std::string buttonName);
            static bool GetMouseButton(int button);
            static bool GetMouseButtonDown(int button);
            static bool GetMouseButtonUp(int button);

        private:
            static std::unordered_map<std::string, int> buttonKeyMap;
            static std::unordered_map<std::string, double> lastKeyPressTime;

            static bool CheckCooldown(std::string buttonName, double cooldown);
    };
}

#endif /* !INPUTSYSTEM_HPP_ */
