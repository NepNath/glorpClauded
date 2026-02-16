#pragma once

#include "ecs/core.hpp"
#include "game.hpp"

#include <SFML/Graphics.hpp>
#include <cmath>
#include <vector>

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

class CollisionSystem : public ecs::System
{
public:
    struct CollisionResult
    {
        std::vector<ecs::Entity> destroyedEnemies;
        std::vector<ecs::Entity> destroyedProjectiles;
        bool playerHit = false;
    };

    CollisionResult update()
    {
        CollisionResult result;

        std::vector<ecs::Entity> projectiles;
        std::vector<ecs::Entity> enemies;
        ecs::Entity playerEntity = 0;
        bool hasPlayer = false;

        for (const auto& entity : entities())
        {
            if (ecs::has_component<main::ProjectileTag>(entity))
                projectiles.push_back(entity);
            else if (ecs::has_component<main::EnemyTag>(entity))
                enemies.push_back(entity);
            else if (ecs::has_component<main::PlayerTag>(entity))
            {
                playerEntity = entity;
                hasPlayer = true;
            }
        }

        if (!hasPlayer)
            return result;

        for (auto proj : projectiles)
        {
            auto& pPos = ecs::get_component<main::Vector2d>(proj);
            if (pPos.x < -100 || pPos.x > 1400 || pPos.y < -100 || pPos.y > 900)
                result.destroyedProjectiles.push_back(proj);
        }

        auto& playerPos = ecs::get_component<main::Vector2d>(playerEntity);
        auto& playerCol = ecs::get_component<main::Collider>(playerEntity);
        auto* playerOff = ecs::try_get_component<main::ColliderOffset>(playerEntity);
        float playerCX = playerPos.x + (playerOff ? playerOff->x : 0.0f);
        float playerCY = playerPos.y + (playerOff ? playerOff->y : 0.0f);

        for (auto enemy : enemies)
        {
            auto& ePos = ecs::get_component<main::Vector2d>(enemy);
            auto& eCol = ecs::get_component<main::Collider>(enemy);
            auto* eOff = ecs::try_get_component<main::ColliderOffset>(enemy);
            float eCX = ePos.x + (eOff ? eOff->x : 0.0f);
            float eCY = ePos.y + (eOff ? eOff->y : 0.0f);

            float dx = eCX - playerCX;
            float dy = eCY - playerCY;
            float dist = std::sqrt(dx * dx + dy * dy);

            if (dist < eCol.radius + playerCol.radius)
            {
                result.playerHit = true;
                result.destroyedEnemies.push_back(enemy);
                continue;
            }

            for (auto proj : projectiles)
            {
                auto& pPos = ecs::get_component<main::Vector2d>(proj);
                auto& pCol = ecs::get_component<main::Collider>(proj);

                float pdx = eCX - pPos.x;
                float pdy = eCY - pPos.y;
                float pdist = std::sqrt(pdx * pdx + pdy * pdy);

                if (pdist < eCol.radius + pCol.radius)
                {
                    result.destroyedEnemies.push_back(enemy);
                    result.destroyedProjectiles.push_back(proj);
                    break;
                }
            }
        }

        return result;
    }
};

} // namespace game::systems
