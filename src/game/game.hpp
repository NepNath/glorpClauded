#pragma once

#include <SFML/Graphics.hpp>
#include <game_export.h>

namespace game::main
{

struct GAME_EXPORT Vector2d
{
    float x;
    float y;

    Vector2d() : x(0.0f), y(0.0f) {}
    Vector2d(float x, float y) : x(x), y(y) {}
};

struct GAME_EXPORT Movement
{
    Vector2d direction;
    float speed;

    Movement() : direction(0, 0), speed(0) {}
    Movement(Vector2d dir, float spd) : direction(dir), speed(spd) {}
};

struct GAME_EXPORT SpriteRenderer
{
    sf::Sprite sprite;
    SpriteRenderer() = default;
    SpriteRenderer(const sf::Texture& texture) : sprite(texture) {}
};

struct GAME_EXPORT Animation
{
    int frameWidth;
    int frameHeight;
    int currentFrame;
    int maxFrames;
    int directionIndex;
    float timer;
    float speed;

    Animation(int w, int h, int max, float spd)
        : frameWidth(w)
        , frameHeight(h)
        , currentFrame(1)
        , maxFrames(max)
        , directionIndex(1)
        , timer(0.f)
        , speed(spd)
    {
    }
    Animation() = default;
};

struct GAME_EXPORT ShapeRenderer
{
    sf::CircleShape shape;

    ShapeRenderer(float radius, sf::Color color)
        : shape(radius)
    {
        shape.setFillColor(color);
        shape.setOrigin(radius, radius);
    }
    ShapeRenderer() = default;
};

struct GAME_EXPORT PlayerTag {};
struct GAME_EXPORT EnemyTag {};
struct GAME_EXPORT ProjectileTag {};

struct GAME_EXPORT Health
{
    int hp;
    Health() : hp(3) {}
    Health(int value) : hp(value) {}
};

struct GAME_EXPORT Collider
{
    float radius;
    Collider() : radius(16.0f) {}
    Collider(float r) : radius(r) {}
};

struct GAME_EXPORT ColliderOffset
{
    float x;
    float y;
    ColliderOffset() : x(0.0f), y(0.0f) {}
    ColliderOffset(float x, float y) : x(x), y(y) {}
};

GAME_EXPORT int GenerateRandomInt(int min, int max);

GAME_EXPORT int run();

} // namespace game::main
