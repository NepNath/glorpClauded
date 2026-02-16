#pragma once

#include "ecs/core.hpp"
#include "game/game.hpp"

#include <cmath>
#include <vector>

namespace game::systems
{

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
