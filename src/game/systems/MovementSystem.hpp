#pragma once

#include "ecs/core.hpp"
#include "game/game.hpp"

namespace game::systems
{

class MovementSystem : public ecs::System
{
public:
    void update(float dt)
    {
        for (const auto& entity : entities())
        {
            auto& pos = ecs::get_component<main::Vector2d>(entity);
            auto& mov = ecs::get_component<main::Movement>(entity);
            pos.x += mov.direction.x * mov.speed * dt;
            pos.y += mov.direction.y * mov.speed * dt;
        }
    }
};

} // namespace game::systems
