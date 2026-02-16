#pragma once

#include "ecs/core.hpp"
#include "game/game.hpp"

namespace game::systems
{

class AnimationSystem : public ecs::System
{
public:
    void update(float dt)
    {
        for (const auto& entity : entities())
        {
            auto& anim = ecs::get_component<main::Animation>(entity);
            auto& sprite = ecs::get_component<main::SpriteRenderer>(entity);
            auto* mov = ecs::try_get_component<main::Movement>(entity);

            bool isMoving = mov && (mov->direction.x != 0.0f || mov->direction.y != 0.0f);

            if (isMoving)
            {
                anim.timer += dt;
                if (anim.timer >= anim.speed)
                {
                    anim.timer = 0.0f;
                    anim.currentFrame++;
                    if (anim.currentFrame >= anim.maxFrames)
                        anim.currentFrame = 1;
                }
            }

            int rectLeft = anim.currentFrame * anim.frameWidth;
            int rectTop = anim.directionIndex * anim.frameHeight;
            sprite.sprite.setTextureRect(sf::IntRect(rectLeft, rectTop, anim.frameWidth, anim.frameHeight));
        }
    }
};

} // namespace game::systems
