#include "game.hpp"
#include "src/menu/ui/window.hpp"
#include <SFML/Graphics.hpp>
#include <print>
#include <random>
#include <vector>

namespace game::main
{

int GenerateRandomInt(int min, int max)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distrib(min, max);

    return distrib(gen);
}

int run()
{
    // ------ SETUP FENÊTRE ------
    sf::RenderWindow window(sf::VideoMode(1280, 720), "buhh", sf::Style::Default);
    sf::Image icon;
    sf::Texture playerTexture;
    sf::Clock clock;

    // ------ SETUP ANIMATION JOUEUR ------
    const int spriteFrameWidth = 64;
    const int spriteFrameHeight = 64;
    int currentFrame = 1;
    int maxFrame = 8;
    int directionIndex = 0;
    float animationTimer = .0f;
    float animationSpeed = .1f;
    float speed = 200.0f;

    if (!playerTexture.loadFromFile("assets/characterset8.png"))
    {
        std::println("Erreur : Impossible de charger characterset.png");
    }
    sf::Sprite playerSprite(playerTexture);

    // ------ SETUP PROJECTILES ------
    std::vector<Projectile> projectiles;
    float projectileSpawnTimer = 0.0f;
    float projectileSpawnDelay = 0.5f;

    if (icon.loadFromFile("assets/icon.png"))
    {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }
    else
    {
        std::println("Failed to load icon image.");
    }

    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);

    while (window.isOpen())
    {
        bool isMoving = false;
        sf::Time deltaTime = clock.restart();
        float deltaTimeSeconds = deltaTime.asSeconds();

        sf::Vector2f movement(0.0f, 0.0f);
        sf::Event event;

        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    return 0;

                case sf::Event::KeyPressed:
                    if (event.key.code == sf::Keyboard::Escape)
                    {
                        window.close();
                        return 0;
                    }
            }
        }

        // ------ GESTION DU SPAWN DE PROJECTILES ------
        projectileSpawnTimer += deltaTimeSeconds;

        if (projectileSpawnTimer >= projectileSpawnDelay)
        {
            projectileSpawnTimer = 0.0f;

            int randX = GenerateRandomInt(0, 1280);
            int randY = GenerateRandomInt(0, 720);

            projectiles.emplace_back(Vector2d((float)randX, (float)randY), 10.0f);

            std::println("New projectile at: {}, {}", randX, randY);
        }

        // ------ MOUVEMENTS JOUEUR ------
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
        {
            movement.x -= 1.0f;
            isMoving = true;
            directionIndex = 2;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            movement.x += 1.0f;
            isMoving = true;
            directionIndex = 3;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
        {
            movement.y -= 1.0f;
            isMoving = true;
            directionIndex = 4;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            movement.y += 1.0f;
            isMoving = true;
            directionIndex = 1;
        }

        if (isMoving)
        {
            animationTimer += deltaTimeSeconds;
            if (animationTimer >= animationSpeed)
            {
                animationTimer = 0.0f;
                currentFrame++;
                if (currentFrame >= maxFrame)
                    currentFrame = 1;
            }
        }

        int rectLeft = currentFrame * spriteFrameWidth;
        int rectTop = directionIndex * spriteFrameWidth;

        playerSprite.setTextureRect(sf::IntRect(rectLeft, rectTop, spriteFrameWidth, spriteFrameHeight));
        std::println("{0}", directionIndex);
        playerSprite.move(movement * speed * deltaTimeSeconds);

        window.clear(sf::Color::Black);

        // ------ SPAWN DES PROJECTILES ------
        for (const auto& proj : projectiles)
        {
            window.draw(proj.shape);
        }

        window.draw(playerSprite);
        window.display();
    }
    return 0;
}
} // namespace game::main