#pragma once

#include "ecs/core.hpp"
#include "game/game.hpp"

#include <SFML/Graphics.hpp>

namespace game::systems
{

class SpriteRenderSystem : public ecs::System
{
public:
    void render(sf::RenderWindow& window)
    {
        for (const auto& entity : entities())
        {
            auto& sprite = ecs::get_component<main::SpriteRenderer>(entity);
            auto& pos = ecs::get_component<main::Vector2d>(entity);
            sprite.sprite.setPosition(pos.x, pos.y);
            window.draw(sprite.sprite);
        }
    }
};

class ShapeRenderSystem : public ecs::System
{
public:
    void render(sf::RenderWindow& window)
    {
        for (const auto& entity : entities())
        {
            auto& shape = ecs::get_component<main::ShapeRenderer>(entity);
            auto& pos = ecs::get_component<main::Vector2d>(entity);
            shape.shape.setPosition(pos.x, pos.y);
            window.draw(shape.shape);
        }
    }
};

} // namespace game::systems
