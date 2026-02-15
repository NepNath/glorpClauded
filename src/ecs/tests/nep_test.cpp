#include <gtest/gtest.h>

#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"
#include "game/game.hpp"

namespace
{
	struct Position : public game::main::Vector2d{};

    struct Motion
    {
        game::main::Vector2d direction;
        float speed;
    };

    class MovementSystem : public ecs::System
    {
        public:
        void updatePosition()
        {
            for (const auto& entity : entities())
            {
                auto& pos = ecs::get_component<Position>(entity);
                auto& mov = ecs::get_component<Motion>(entity);
                pos.x += mov.direction.x * mov.speed * 1.0f;
                pos.y += mov.direction.y * mov.speed * 1.0f;
            }
        }
        std::set<ecs::Entity>& get_entities()
        {
            return entities();
        }
        
    };

    TEST(CoreEcsTestSuite, test)
    {
        ecs::register_component<Position>();
        ecs::register_component<Motion>();

        auto movement = std::make_shared<MovementSystem>();

        ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Motion>());

        ecs::Entity e = ecs::create_entity();

        ecs::add_components(e, Position{{0.0f, 0.0f}}, Motion{game::main::Vector2d{1.0f, 0.0f}, 4.0f});
        ecs::get_system<MovementSystem>()->updatePosition();
        auto& pos = ecs::get_component<Position>(e);
        EXPECT_EQ(pos.x, 4.0f);
        EXPECT_EQ(pos.y, 0.0f);
    }
    
}
