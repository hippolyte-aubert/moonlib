/*
** EPITECH PROJECT, 2023
** B-CPP-500-MPL-5-1-rtype-nils.colire
** File description:
** RenderingSystem
*/

#include "RenderingSystem.hpp"

namespace ECS {
    RenderingSystem::RenderingSystem()
    {
    }

    RenderingSystem::~RenderingSystem()
    {
    }

    void RenderingSystem::Start(Coordinator &coordinator, AssetManager &assetManager)
    {
        for (auto const &entity : entities) {
            auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);
            auto &transform = coordinator.GetComponent<Transform2D>(entity);

            auto assetOpt = assetManager.getAsset(renderer2D.textureName);
            if (assetOpt) renderer2D.texture = assetOpt.value()->GetData<Texture2D>();
            else std::cerr << "Texture " << renderer2D.textureName << " not found" << std::endl;

            renderer2D.texture.width *= transform.scale.x;
            renderer2D.texture.height *= transform.scale.y;
            renderer2D.origin = {0, 0};

            if (renderer2D.isAnimated) {
                renderer2D.sourceRec.width = renderer2D.texture.width / renderer2D.nbFrames;
                renderer2D.sourceRec.height = renderer2D.texture.height;
                if (renderer2D.animateOnStart) renderer2D.animate = true;
            } else {
                renderer2D.sourceRec.width = renderer2D.texture.width;
                renderer2D.sourceRec.height = renderer2D.texture.height;
            }
            renderer2D.textureLoaded = true;

            entitiesToSort.push_back(entity);
        }
    }

    void RenderingSystem::Update(Coordinator &coordinator, AssetManager &assetManager)
    {
        if (entities.size() != entitiesToSort.size()) {
            entitiesToSort.clear();
            for (auto const &entity : entities) {
                auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);
                auto &transform = coordinator.GetComponent<Transform2D>(entity);

                if (!renderer2D.textureLoaded) {
                    auto assetOpt = assetManager.getAsset(renderer2D.textureName);
                    if (assetOpt) renderer2D.texture = assetOpt.value()->GetData<Texture2D>();
                    else std::cerr << "Texture " << renderer2D.textureName << " not found" << std::endl;

                    renderer2D.texture.width *= transform.scale.x;
                    renderer2D.texture.height *= transform.scale.y;

                    if (renderer2D.isAnimated) {
                        renderer2D.sourceRec.width = renderer2D.texture.width / renderer2D.nbFrames;
                        renderer2D.sourceRec.height = renderer2D.texture.height;
                        if (renderer2D.animateOnStart) renderer2D.animate = true;
                    } else {
                        renderer2D.sourceRec.width = renderer2D.texture.width;
                        renderer2D.sourceRec.height = renderer2D.texture.height;
                    }

                    renderer2D.textureLoaded = true;
                }

                entitiesToSort.push_back(entity);
            }
        }

        std::sort(entitiesToSort.begin(), entitiesToSort.end(), [&](Entity &e1, Entity &e2) {
            auto &r1 = coordinator.GetComponent<Renderer2D>(e1);
            auto &r2 = coordinator.GetComponent<Renderer2D>(e2);
            return r1.zIndex > r2.zIndex;
        });

        for (auto const &entity : entitiesToSort) {
            auto &transform = coordinator.GetComponent<Transform2D>(entity);
            auto &renderer2D = coordinator.GetComponent<Renderer2D>(entity);

            if (renderer2D.draw) {
                if (renderer2D.isAnimated) AnimateSprite(renderer2D, entity, coordinator);
                DrawTexturePro(renderer2D.texture, renderer2D.sourceRec, {transform.position.x, transform.position.y, (float)renderer2D.texture.width / renderer2D.nbFrames, (float)renderer2D.texture.height}, {0, 0}, transform.rotation, renderer2D.tint);
            }
        }
    }

    void RenderingSystem::AnimateSprite(Renderer2D &renderer2D, Entity id, Coordinator &coordinator)
    {
        if (!renderer2D.loopAnimation) {
            if (renderer2D.animate) {
                renderer2D.currentFrameCounter++;
                if (renderer2D.currentFrameCounter >= GetFPS() / renderer2D.framesSpeed) {
                    renderer2D.currentFrame++;
                    renderer2D.currentFrameCounter = 0;
                    if (renderer2D.currentFrame >= renderer2D.nbFrames - 1) {
                        renderer2D.animate = false;
                        if (renderer2D.destroyOnAnimationEnd) coordinator.DestroyEntity(id);
                    }
                    renderer2D.sourceRec.x = (float)renderer2D.currentFrame * ((float)renderer2D.texture.width / renderer2D.nbFrames);
                }
            }
        } else {
            renderer2D.currentFrameCounter++;
            if (renderer2D.currentFrameCounter >= GetFPS() / renderer2D.framesSpeed) {
                renderer2D.currentFrameCounter = 0;
                renderer2D.currentFrame++;
                if (renderer2D.currentFrame >= renderer2D.nbFrames) renderer2D.currentFrame = 0;
                renderer2D.sourceRec.x = (float)renderer2D.currentFrame * (float)renderer2D.texture.width / renderer2D.nbFrames;
            }
        }
    }
}