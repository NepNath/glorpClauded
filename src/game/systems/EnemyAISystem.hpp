#pragma once

#include "ecs/core.hpp"
#include "game/game.hpp"

#include <cmath>

namespace game::systems
{

class EnemyAISystem : public ecs::System
{
public:
    void update(const main::Vector2d& playerPos)
    {
        for (const auto& entity : entities())
        {
            auto& pos = ecs::get_component<main::Vector2d>(entity);
            auto& mov = ecs::get_component<main::Movement>(entity);

            float dx = playerPos.x - pos.x;
            float dy = playerPos.y - pos.y;
            float len = std::sqrt(dx * dx + dy * dy);

            if (len > 0.0f)
            {
                mov.direction.x = dx / len;
                mov.direction.y = dy / len;
            }
        }
    }
};

} // namespace game::systems
