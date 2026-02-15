#pragma once

#include <SFML/Graphics.hpp>
#include <game_export.h>

namespace game::main
{
struct GAME_EXPORT Vector2d
{
    float x;
    float y;

    Vector2d(): x(0.0f), y(0.0f){}
    Vector2d(float x, float y): x(x), y(y){}
};

struct GAME_EXPORT Movement
{
    Vector2d direction;
    float speed;

    Movement() : direction(0, 0), speed(0){}
    Movement(Vector2d dir, float spd): direction(dir), speed(spd){}
};

struct GAME_EXPORT Projectile
{
    Vector2d position;
    sf::CircleShape shape;
    Projectile(Vector2d pos, float size): position(pos), shape(size)
    {
        shape.setPosition(pos.x, pos.y);
        shape.setFillColor(sf::Color::Red);
    }
};

struct GAME_EXPORT SpriteRenderer
{
    sf::Sprite sprite;
    SpriteRenderer() = default;
    SpriteRenderer(const sf::Texture& texture): sprite(texture){}
};

struct GAME_EXPORT Animation
{
    int frameWidth;     // 64
    int frameHeight;    // 64
    int currentFrame;   è// 1
    int maxFrames;      // 8
    int directionIndex; // 1=Bas, 2=Gauche, 3=Droite, 4=Haut possibilité de faire un ENUM
    float timer;        // 0.f
    float speed;        // .1f

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

GAME_EXPORT int GenerateRandomInt(int min, int max);

GAME_EXPORT int run();
} // namespace game::main