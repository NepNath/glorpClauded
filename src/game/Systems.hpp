#pragma once

#include "src/ecs/core.hpp"
#include "src/ecs/internal/system_manager.hpp"

#include "game.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>



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
    class InputSystem : public ecs::System
    {
        public:
            void update()
            {
                for (const auto& entity : entities())
                {
                    auto& mov = ecs::get_component<main::Movement>(entity);
                    auto& anim = ecs::get_component<main::Animation>(entity);

                    bool isMoving = false;

                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) || sf::Keyboard::isKeyPressed(sf::Keyboard::Up) /*|| sf::Joystick::getAxisPosition(0, sf::Joystick::Axis::Y*/)
                    {
                        anim.directionIndex = 4;
                        mov.direction.x -= 1.0f;
                        isMoving = true;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) || sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
                    {
                        anim.directionIndex = 1;
                        mov.direction.x += 1.0f;
                        isMoving = true;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q) || sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
                    {
                        anim.directionIndex = 2;
                        mov.direction.y += 1.0f;
                        isMoving = true;
                    }
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) || sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
                    {
                        anim.directionIndex = 3;
                        mov.direction.y -= 1.0f;
                        isMoving = true;
                    }

                    if (isMoving && mov.direction.x != .0f && mov.direction.y != .0f)
                    {
                        // voir pour normaliser les diagonales  
                    }
                }   

            }
    };
}