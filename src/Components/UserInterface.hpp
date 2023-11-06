/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** UI
*/

#ifndef USERINTERFACE_HPP_
    #define USERINTERFACE_HPP__HPP_

    #include <iostream>
    #include <functional>
    #include <raylib.h>

namespace ECS {
    struct Text {
        std::string text;
        std::string fontName;
        Vector2 position;
        float fontSize = 12;
        float spacing = 1;
        Color color = WHITE;
        Font font = {0};
        bool fontLoaded = false;
    };

    struct Button {
        Vector2 position;
        float width;
        float height;
        Text buttonText;
        Color baseColor = WHITE;
        Color overColor = GRAY;
        Color clickColor = DARKGRAY;
        bool rounded = false;
        float roundness = 1.0f;
        Rectangle rect = {0, 0, 0, 0};
        bool set = false;

        void OnClick(std::function<void()> callback)
        {
            static Color storeColor = baseColor;
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rect)) {
                callback();
                baseColor = clickColor;
            }
        }

        void OnOver()
        {
            static Color storeColor = baseColor;
            if (CheckCollisionPointRec(GetMousePosition(), rect)) {
                baseColor = overColor;
            } else {
                baseColor = storeColor;
            }
        }
    };
}

// struct InputText {
//     Text placeHolder = {0};
//     Text mInputText;
//     std::string placeholderText;
//     std::string input = "";
//     bool typing = false;
//     bool selected = false;
//     Rectangle rect = {0, 0, 0, 0};
//     Color baseColor = WHITE;
//     Color overColor = GRAY;
//     Color clickColor = DARKGRAY;
//     bool rounded = false;
//     float roundness = 1.0f;
//     Vector2 position;
//     float width;
//     float height;

//     void onClick()
//     {
//         static Color storeColor = baseColor;
//         if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && CheckCollisionPointRec(GetMousePosition(), rect)) {
//             typing = true;
//             baseColor = clickColor;
//         } else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !CheckCollisionPointRec(GetMousePosition(), rect)) {
//             typing = false;
//             baseColor = storeColor;
//         }
//     }

//     void onOver()
//     {
//         static Color storeColor = baseColor;
//         if (CheckCollisionPointRec(GetMousePosition(), rect)) {
//             baseColor = overColor;
//         } else {
//             baseColor = storeColor;
//         }
//     }

//     void onTyping()
//     {
//         int key = GetKeyPressed();
//         if (typing) {
//             while (key > 0) {
//                 if ((key >= 32) && (key <= 125) && (input.length() < 20)) {
//                     input += (char)key;
//                 }
//                 key = GetKeyPressed();
//             }
//             std::transform(input.begin(), input.end(), input.begin(), ::tolower);
//             if (IsKeyPressed(KEY_BACKSPACE))
//                 if (input.length() > 0)
//                     input.pop_back();
//             if (IsKeyPressed(KEY_ENTER)) {
//                 if (typing) typing = false;
//             }
//             mInputText.text = input;
//         }
//     }
// };


#endif /* !USERINTERFACE_HPP__HPP_ */
