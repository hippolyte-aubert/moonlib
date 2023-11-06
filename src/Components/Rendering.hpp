/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** Sprite
*/

#ifndef RENDERING_HPP_
    #define RENDERING_HPP_

    #include <iostream>
    #include <raylib.h>

namespace ECS {
    struct Renderer2D {
        std::string textureName;
        Color tint = WHITE;
        bool draw = true;
        bool textureLoaded = false;
        Texture2D texture = {0};
        int zIndex = 0;
        Vector2 origin = {0, 0};
        bool isAnimated = false;
        int nbFrames = 1;
        int framesSpeed = 0;
        int currentFrame = 0;
        int currentFrameCounter = 0;
        bool animate = false;
        bool loopAnimation = false;
        bool animateOnStart = false;
        bool destroyOnAnimationEnd = false;
        Rectangle sourceRec = {0};
    };
}

#endif /* !RENDERING_HPP_ */
