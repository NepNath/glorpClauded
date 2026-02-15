#include <gtest/gtest.h>

#include "ecs/core.hpp"
#include "ecs/internal/system_manager.hpp"

namespace
{

struct Position final
{
    float x{}, y{};
};

struct Velocity final
{
    float vx{}, vy{};
};

struct MovementSystem final : public ecs::System
{
    std::set<ecs::Entity>& get_entities()
    {
        return entities();
    }
};

} // namespace

TEST(CoreEcsTestSuite, CreateDestroyEntity)
{
    const ecs::Entity e0 = ecs::create_entity();
    const ecs::Entity e1 = ecs::create_entity();

    EXPECT_LT(e0, ecs::MAX_ENTITIES);
    EXPECT_LT(e1, ecs::MAX_ENTITIES);
    EXPECT_NE(e0, e1);

    ecs::destroy_entity(e0);
    ecs::destroy_entity(e1);
}

TEST(CoreEcsTestSuite, RegisterComponentAndUpdateEntity)
{
    ecs::register_component<Position>();
    ecs::register_component<Velocity>();

    const ecs::Entity e = ecs::create_entity();
    ecs::add_components(e, Position{1.f, 2.f}, Velocity{.vx=0.2, .vy=0.5});

    auto& p = ecs::get_component<Position>(e);
    EXPECT_FLOAT_EQ(p.x, 1.f);
    EXPECT_FLOAT_EQ(p.y, 2.f);

    ecs::remove_components<Position>(e);

    ecs::destroy_entity(e);
}

TEST(CoreEcsTestSuite, AddingSameComponentTwiceAsserts)
{
    ecs::register_component<Position>();

    const ecs::Entity e = ecs::create_entity();
    ecs::add_components(e, Position{1.f, 2.f});

    EXPECT_DEBUG_DEATH(ecs::add_components(e, Position{3.f, 4.f}), "already exists");

    ecs::destroy_entity(e);
}

TEST(CoreEcsTestSuite, RegisterSystemAndChangeEntityComponentsToChangeMembership)
{
    ecs::register_component<Position>();
    ecs::register_component<Velocity>();

    auto movement = std::make_shared<MovementSystem>();

    ecs::register_system<MovementSystem>(movement, ecs::create_signature<Position, Velocity>());

    const ecs::Entity e = ecs::create_entity();
    EXPECT_TRUE(movement->get_entities().empty()) << "should not be in system yet";

    ecs::add_components(e, Position{0.f, 0.f});
    EXPECT_TRUE(movement->get_entities().empty()) << "should be missing Velocity component to be in system";

    ecs::add_components(e, Velocity{1.f, 1.f});
    EXPECT_EQ(movement->get_entities().count(e), 1u) << "Position and Velocity components should be enough";

    ecs::remove_components<Velocity>(e);
    EXPECT_EQ(movement->get_entities().count(e), 0u) << "Removing Velocity component should remove entity from system";

    ecs::destroy_entity(e);
}
