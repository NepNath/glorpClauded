#pragma once

#include "ecs/core.hpp"
#include "game/game.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>

namespace game::systems
{

class InputSystem : public ecs::System
{
public:
    void update()
    {
        for (const auto& entity : entities())
        {
            auto& mov = ecs::get_component<main::Movement>(entity);
            auto& anim = ecs::get_component<main::Animation>(entity);

            mov.direction = main::Vector2d(0, 0);

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                mov.direction.x = -1.0f;
                anim.directionIndex = 2;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                mov.direction.x = 1.0f;
                anim.directionIndex = 3;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                mov.direction.y = -1.0f;
                anim.directionIndex = 4;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                mov.direction.y = 1.0f;
                anim.directionIndex = 1;
            }

            if (mov.direction.x != 0.0f && mov.direction.y != 0.0f)
            {
                float len = std::sqrt(mov.direction.x * mov.direction.x + mov.direction.y * mov.direction.y);
                mov.direction.x /= len;
                mov.direction.y /= len;
            }
        }
    }
};

} // namespace game::systems
