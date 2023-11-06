/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** UserInterfaceSystem
*/

#include "UserInterfaceSystem.hpp"

namespace ECS {
    void TextSystem::Start(Coordinator &coordinator, AssetManager &assetManager)
    {
        for (auto const &entity : entities) {
            auto &text = coordinator.GetComponent<Text>(entity);

            auto assetOpt = assetManager.getAsset(text.fontName);
            if (assetOpt) {
                text.font = assetOpt.value()->GetData<Font>();
                text.fontLoaded = true;
            } else {
                std::cerr << "Font " << text.fontName << " not found" << std::endl;
            }
        }
    }

    void TextSystem::Update(Coordinator &coordinator, AssetManager &assetManager)
    {
        for (auto const &entity : entities) {
            auto &text = coordinator.GetComponent<Text>(entity);
            if (!text.fontLoaded) {
                auto assetOpt = assetManager.getAsset(text.fontName);
                if (assetOpt) {
                    text.font = assetOpt.value()->GetData<Font>();
                    text.fontLoaded = true;
                } else {
                    std::cerr << "Font " << text.fontName << " not found" << std::endl;
                }
            }

            DrawTextEx(text.font, text.text.c_str(), text.position, text.fontSize, text.spacing, text.color);
        }
    }

    void ButtonSystem::Start(Coordinator &coordinator, AssetManager &assetManager)
    {
        for (auto const &entity : entities) {
            auto &button = coordinator.GetComponent<Button>(entity);

            button.rect = {button.position.x, button.position.y, button.width, button.height};

            auto assetOpt = assetManager.getAsset(button.buttonText.fontName);
            if (assetOpt) button.buttonText.font = assetOpt.value()->GetData<Font>();
            else std::cerr << "Font " << button.buttonText.fontName << " not found" << std::endl;

            button.buttonText.position = {
                button.position.x + button.width / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).x / 2, 
                button.position.y + button.height / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).y / 2
            };
            button.buttonText.fontLoaded = true;
            button.set = true;
        }
    }

    void ButtonSystem::Update(Coordinator &coordinator, AssetManager &assetManager)
    {
        for (auto const &entity : entities) {
            auto &button = coordinator.GetComponent<Button>(entity);

            if (!button.set || !button.buttonText.fontLoaded) {
                button.rect = {button.position.x, button.position.y, button.width, button.height};
                auto assetOpt = assetManager.getAsset(button.buttonText.fontName);
                if (assetOpt) button.buttonText.font = assetOpt.value()->GetData<Font>();
                else std::cerr << "Font " << button.buttonText.fontName << " not found" << std::endl;

                button.buttonText.position = {
                    button.position.x + button.width / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).x / 2, 
                    button.position.y + button.height / 2 - MeasureTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.fontSize, button.buttonText.spacing).y / 2
                };
                button.buttonText.fontLoaded = true;
                button.set = true;
            }

            if (button.rounded) {
                DrawRectangleRounded(button.rect, button.roundness, 0, button.baseColor);
                DrawTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.position, button.buttonText.fontSize, button.buttonText.spacing, button.buttonText.color);
            } else {
                DrawRectangleRec(button.rect, button.baseColor);
                DrawTextEx(button.buttonText.font, button.buttonText.text.c_str(), button.buttonText.position, button.buttonText.fontSize, button.buttonText.spacing, button.buttonText.color);
            }
        }
    }
}

// void InputTextSystem::Start(Coordinator &coordinator)
// {
//     for (auto const &entity : entities) {
//         auto &inputText = coordinator.GetComponent<InputText>(entity);

//         inputText.rect = {inputText.position.x, inputText.position.y, inputText.width, inputText.height};
//         inputText.mInputText.font = LoadFontEx(inputText.mInputText.fontPath.c_str(), inputText.mInputText.fontSize, 0, 250);
//         inputText.mInputText.position = {
//             inputText.position.x + inputText.width / 2 - MeasureTextEx(inputText.mInputText.font, inputText.mInputText.text.c_str(), inputText.mInputText.fontSize, inputText.mInputText.spacing).x / 2, 
//             inputText.position.y + inputText.height / 2 - MeasureTextEx(inputText.mInputText.font, inputText.mInputText.text.c_str(), inputText.mInputText.fontSize, inputText.mInputText.spacing).y / 2
//         };
//         inputText.placeHolder.font = LoadFontEx(inputText.mInputText.fontPath.c_str(), inputText.placeHolder.fontSize, 0, 250);
//         inputText.placeHolder.position = {inputText.mInputText.position.x, inputText.mInputText.position.y};
//         inputText.placeHolder.text = inputText.placeholderText;
//         inputText.placeHolder.color = GRAY;
//     }
// }

// void InputTextSystem::Update(Coordinator &coordinator)
// {
//     for (auto const &entity : entities) {
//         auto &inputText = coordinator.GetComponent<InputText>(entity);

//         if (inputText.rounded) {
//             DrawRectangleRounded(inputText.rect, inputText.roundness, 0, inputText.baseColor);
//             if (!inputText.typing && inputText.input.empty()) {
//                 DrawTextEx(inputText.placeHolder.font, inputText.placeHolder.text.c_str(), inputText.placeHolder.position, inputText.placeHolder.fontSize, inputText.placeHolder.spacing, inputText.placeHolder.color);
//             } else {
//                 DrawTextEx(inputText.mInputText.font, inputText.input.c_str(), inputText.mInputText.position, inputText.mInputText.fontSize, inputText.mInputText.spacing, inputText.mInputText.color);
//                 inputText.onTyping();
//             }
//         } else {
//             DrawRectangleRec(inputText.rect, inputText.baseColor);
//             if (!inputText.typing && inputText.input.empty()) {
//                 DrawTextEx(inputText.placeHolder.font, inputText.placeHolder.text.c_str(), inputText.placeHolder.position, inputText.placeHolder.fontSize, inputText.placeHolder.spacing, inputText.placeHolder.color);
//             } else {
//                 DrawTextEx(inputText.mInputText.font, inputText.input.c_str(), inputText.mInputText.position, inputText.mInputText.fontSize, inputText.mInputText.spacing, inputText.mInputText.color);
//                 inputText.onTyping();
//             }
//         }
//         inputText.onOver();
//         inputText.onClick();
//     }
// }
