/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** InputSystems
*/

#include "InputSystem.hpp"

namespace ECS {
    std::unordered_map<std::string, int> InputSystem::buttonKeyMap = {
        {"0", KEY_ZERO},
        {"1", KEY_ONE},
        {"2", KEY_TWO},
        {"3", KEY_THREE},
        {"4", KEY_FOUR},
        {"5", KEY_FIVE},
        {"6", KEY_SIX},
        {"7", KEY_SEVEN},
        {"8", KEY_EIGHT},
        {"9", KEY_NINE},
        {"A", KEY_A},
        {"B", KEY_B},
        {"C", KEY_C},
        {"D", KEY_D},
        {"E", KEY_E},
        {"F", KEY_F},
        {"G", KEY_G},
        {"H", KEY_H},
        {"I", KEY_I},
        {"J", KEY_J},
        {"K", KEY_K},
        {"L", KEY_L},
        {"M", KEY_M},
        {"N", KEY_N},
        {"O", KEY_O},
        {"P", KEY_P},
        {"Q", KEY_Q},
        {"R", KEY_R},
        {"S", KEY_S},
        {"T", KEY_T},
        {"U", KEY_U},
        {"V", KEY_V},
        {"W", KEY_W},
        {"X", KEY_X},
        {"Y", KEY_Y},
        {"Z", KEY_Z},
        {"APOSTROPHE", KEY_APOSTROPHE},
        {"COMMA", KEY_COMMA},
        {"MINUS", KEY_MINUS},
        {"PERIOD", KEY_PERIOD},
        {"SLASH", KEY_SLASH},
        {"SEMICOLON", KEY_SEMICOLON},
        {"EQUAL", KEY_EQUAL},
        {"LEFT_BRACKET", KEY_LEFT_BRACKET},
        {"RIGHT_BRACKET", KEY_RIGHT_BRACKET},
        {"BACKSLASH", KEY_BACKSLASH},
        {"GRAVE", KEY_GRAVE},
        {"SPACE", KEY_SPACE},
        {"ESPACE", KEY_ESCAPE},
        {"ENTER", KEY_ENTER},
        {"TAB", KEY_TAB},
        {"BACKSPACE", KEY_BACKSPACE},
        {"INSERT", KEY_INSERT},
        {"DELETE", KEY_DELETE},
        {"RIGHT", KEY_RIGHT},
        {"LEFT", KEY_LEFT},
        {"DOWN", KEY_DOWN},
        {"UP", KEY_UP},
        {"PAGEUP", KEY_PAGE_UP},
        {"PAGEDOWN", KEY_PAGE_DOWN},
        {"HOME", KEY_HOME},
        {"END", KEY_END},
        {"CAPSLOCK", KEY_CAPS_LOCK},
        {"SCROLLLOCK", KEY_SCROLL_LOCK},
        {"NUMLOCK", KEY_NUM_LOCK},
        {"PRINTSCREEN", KEY_PRINT_SCREEN},
        {"PAUSE", KEY_PAUSE},
        {"F1", KEY_F1},
        {"F2", KEY_F2},
        {"F3", KEY_F3},
        {"F4", KEY_F4},
        {"F5", KEY_F5},
        {"F6", KEY_F6},
        {"F7", KEY_F7},
        {"F8", KEY_F8},
        {"F9", KEY_F9},
        {"F10", KEY_F10},
        {"F11", KEY_F11},
        {"F12", KEY_F12},
        {"LEFTSHIFT", KEY_LEFT_SHIFT},
        {"LEFTCONTROL", KEY_LEFT_CONTROL},
        {"LEFTALT", KEY_LEFT_ALT},
        {"RIGHTSHIFT", KEY_RIGHT_SHIFT},
        {"RIGHTCONTROL", KEY_RIGHT_CONTROL},
        {"RIGHTALT", KEY_RIGHT_ALT},
        {"MOUSELEFT", MOUSE_LEFT_BUTTON},
        {"MOUSERIGHT", MOUSE_RIGHT_BUTTON},
        {"MOUSEMIDDLE", MOUSE_MIDDLE_BUTTON},
    };

    std::unordered_map<std::string, double> InputSystem::lastKeyPressTime = {
        {"0", 0.0f},
        {"1", 0.0f},
        {"2", 0.0f},
        {"3", 0.0f},
        {"4", 0.0f},
        {"5", 0.0f},
        {"6", 0.0f},
        {"7", 0.0f},
        {"8", 0.0f},
        {"9", 0.0f},
        {"A", 0.0f},
        {"B", 0.0f},
        {"C", 0.0f},
        {"D", 0.0f},
        {"E", 0.0f},
        {"F", 0.0f},
        {"G", 0.0f},
        {"H", 0.0f},
        {"I", 0.0f},
        {"J", 0.0f},
        {"K", 0.0f},
        {"L", 0.0f},
        {"M", 0.0f},
        {"N", 0.0f},
        {"O", 0.0f},
        {"P", 0.0f},
        {"Q", 0.0f},
        {"R", 0.0f},
        {"S", 0.0f},
        {"T", 0.0f},
        {"U", 0.0f},
        {"V", 0.0f},
        {"W", 0.0f},
        {"X", 0.0f},
        {"Y", 0.0f},
        {"Z", 0.0f},
        {"APOSTROPHE", 0.0f},
        {"COMMA", 0.0f},
        {"MINUS", 0.0f},
        {"PERIOD", 0.0f},
        {"SLASH", 0.0f},
        {"SEMICOLON", 0.0f},
        {"EQUAL", 0.0f},
        {"LEFT_BRACKET", 0.0f},
        {"RIGHT_BRACKET", 0.0f},
        {"BACKSLASH", 0.0f},
        {"GRAVE", 0.0f},
        {"SPACE", 0.0f},
        {"ESPACE", 0.0f},
        {"ENTER", 0.0f},
        {"TAB", 0.0f},
        {"BACKSPACE", 0.0f},
        {"INSERT", 0.0f},
        {"DELETE", 0.0f},
        {"RIGHT", 0.0f},
        {"LEFT", 0.0f},
        {"DOWN", 0.0f},
        {"UP", 0.0f},
        {"PAGEUP", 0.0f},
        {"PAGEDOWN", 0.0f},
        {"HOME", 0.0f},
        {"END", 0.0f},
        {"CAPSLOCK", 0.0f},
        {"SCROLLLOCK", 0.0f},
        {"NUMLOCK", 0.0f},
        {"PRINTSCREEN", 0.0f},
        {"PAUSE", 0.0f},
        {"F1", 0.0f},
        {"F2", 0.0f},
        {"F3", 0.0f},
        {"F4", 0.0f},
        {"F5", 0.0f},
        {"F6", 0.0f},
        {"F7", 0.0f},
        {"F8", 0.0f},
        {"F9", 0.0f},
        {"F10", 0.0f},
        {"F11", 0.0f},
        {"F12", 0.0f},
        {"LEFTSHIFT", 0.0f},
        {"LEFTCONTROL", 0.0f},
        {"LEFTALT", 0.0f},
        {"RIGHTSHIFT", 0.0f},
        {"RIGHTCONTROL", 0.0f},
        {"RIGHTALT", 0.0f},
    };

    Vector2 InputSystem::mousePosition()
    {
        return GetMousePosition();
    }

    Vector2 InputSystem::mouseScroll()
    {
        return GetMouseWheelMoveV();
    }

    float InputSystem::GetAxis(std::string axisName)
    {
        if (axisName == "Horizontal")
            return IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT);
        else if (axisName == "Vertical")
            return IsKeyDown(KEY_DOWN) - IsKeyDown(KEY_UP);
        else {
            std::cerr << "Axis name not found" << std::endl;
            return 0;
        }
    }

    bool InputSystem::GetKey(std::string buttonName)
    {
        std::transform(buttonName.begin(), buttonName.end(), buttonName.begin(), ::toupper);

        if (buttonKeyMap.find(buttonName) != buttonKeyMap.end()) {
            static std::unordered_map<std::string, float> lastPressTimes;
            int key = buttonKeyMap[buttonName];

            double currentTime = GetTime();
            if (lastPressTimes.find(buttonName) == lastPressTimes.end() ||
                currentTime - lastPressTimes[buttonName] >= 0.3) {
                if (IsKeyDown(key)) {
                    lastPressTimes[buttonName] = currentTime;
                    return true;
                }
            }
            return false;
        } else {
            std::cerr << "Button name not found: " << buttonName << std::endl;
            return false;
        }
    }

    bool InputSystem::GetKeyDown(std::string buttonName)
    {
        std::transform(buttonName.begin(), buttonName.end(), buttonName.begin(), ::toupper);

        if (buttonKeyMap.find(buttonName) != buttonKeyMap.end()) {
            int key = buttonKeyMap[buttonName];
            return IsKeyDown(key);
        } else {
            std::cerr << "Button name not found: " << buttonName << std::endl;
            return false;
        }
    }

    bool InputSystem::GetKeyUp(std::string buttonName)
    {
        std::transform(buttonName.begin(), buttonName.end(), buttonName.begin(), ::toupper);

        if (buttonKeyMap.find(buttonName) != buttonKeyMap.end()) {
            int key = buttonKeyMap[buttonName];
            return IsKeyReleased(key);
        } else {
            std::cerr << "Button name not found: " << buttonName << std::endl;
            return false;
        }
    }

    bool InputSystem::GetMouseButton(int button)
    {
        return IsMouseButtonPressed(button);
    }

    bool InputSystem::GetMouseButtonDown(int button)
    {
        return IsMouseButtonDown(button);
    }

    bool InputSystem::GetMouseButtonUp(int button)
    {
        return IsMouseButtonReleased(button);
    }

    bool InputSystem::CheckCooldown(std::string buttonName, double cooldown)
    {
        double currentTime = GetTime();
        if (lastKeyPressTime.find(buttonName) == lastKeyPressTime.end() ||
            currentTime - lastKeyPressTime[buttonName] >= cooldown) {
            lastKeyPressTime[buttonName] = currentTime;
            return true;
        }
        return false;
    }
}
