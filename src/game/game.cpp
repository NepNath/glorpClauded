#include "game.hpp"
#include "Systems.hpp"
#include "ecs/core.hpp"

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <memory>
#include <random>
#include <set>
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
    sf::RenderWindow window(sf::VideoMode(1280, 720), "GlorpSurvivor", sf::Style::Default);
    sf::Image icon;
    sf::Clock gameClock;
    sf::Font font;

    if (icon.loadFromFile("assets/icon.png"))
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    font.loadFromFile("assets/font.ttf");
    window.setFramerateLimit(144);
    window.setVerticalSyncEnabled(true);

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("assets/background.png");
    sf::Sprite backgroundSprite(backgroundTexture);
    float bgScaleX = 1280.0f / static_cast<float>(backgroundTexture.getSize().x);
    float bgScaleY = 720.0f / static_cast<float>(backgroundTexture.getSize().y);
    backgroundSprite.setScale(bgScaleX, bgScaleY);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("assets/characterset8.png");

    std::vector<sf::Texture> enemyTextures(7);
    enemyTextures[0].loadFromFile("assets/ennemies/blorp-3x.png");
    enemyTextures[1].loadFromFile("assets/ennemies/glorp-3x.png");
    enemyTextures[2].loadFromFile("assets/ennemies/olorp-3x.png");
    enemyTextures[3].loadFromFile("assets/ennemies/plorp-3x.png");
    enemyTextures[4].loadFromFile("assets/ennemies/rlorp-3x.png");
    enemyTextures[5].loadFromFile("assets/ennemies/wlorp-3x.png");
    enemyTextures[6].loadFromFile("assets/ennemies/ylorp-3x.png");

    ecs::register_component<Vector2d>();
    ecs::register_component<Movement>();
    ecs::register_component<SpriteRenderer>();
    ecs::register_component<Animation>();
    ecs::register_component<ShapeRenderer>();
    ecs::register_component<PlayerTag>();
    ecs::register_component<EnemyTag>();
    ecs::register_component<ProjectileTag>();
    ecs::register_component<Health>();
    ecs::register_component<Collider>();
    ecs::register_component<ColliderOffset>();

    auto inputSystem = std::make_shared<systems::InputSystem>();
    ecs::register_system(inputSystem, ecs::create_signature<Movement, Animation, PlayerTag>());

    auto movementSystem = std::make_shared<systems::MovementSystem>();
    ecs::register_system(movementSystem, ecs::create_signature<Vector2d, Movement>());

    auto animationSystem = std::make_shared<systems::AnimationSystem>();
    ecs::register_system(animationSystem, ecs::create_signature<Animation, SpriteRenderer>());

    auto enemyAISystem = std::make_shared<systems::EnemyAISystem>();
    ecs::register_system(enemyAISystem, ecs::create_signature<EnemyTag, Vector2d, Movement>());

    auto spriteRenderSystem = std::make_shared<systems::SpriteRenderSystem>();
    ecs::register_system(spriteRenderSystem, ecs::create_signature<SpriteRenderer, Vector2d>());

    auto shapeRenderSystem = std::make_shared<systems::ShapeRenderSystem>();
    ecs::register_system(shapeRenderSystem, ecs::create_signature<ShapeRenderer, Vector2d>());

    auto collisionSystem = std::make_shared<systems::CollisionSystem>();
    ecs::register_system(collisionSystem, ecs::create_signature<Vector2d, Collider>());

    ecs::Entity player = ecs::create_entity();
    ecs::add_components(player,
        Vector2d(608.0f, 328.0f),
        Movement(Vector2d(0, 0), 200.0f),
        SpriteRenderer(playerTexture),
        Animation(64, 64, 8, 0.1f),
        PlayerTag{},
        Health(5),
        Collider(20.0f),
        ColliderOffset(32.0f, 40.0f)
    );

    float enemySpawnTimer = 0.0f;
    float enemySpawnDelay = 2.0f;
    float shootCooldown = 0.0f;
    float invincibilityTimer = 0.0f;
    int score = 0;
    bool gameOver = false;
    Vector2d lastShootDir(0, -1);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10.0f, 10.0f);

    sf::Text healthText;
    healthText.setFont(font);
    healthText.setCharacterSize(20);
    healthText.setFillColor(sf::Color::Red);
    healthText.setPosition(10.0f, 40.0f);

    sf::Text gameOverText;
    gameOverText.setFont(font);
    gameOverText.setCharacterSize(50);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(450.0f, 250.0f);
    gameOverText.setString("GAME OVER");

    sf::Text restartText;
    restartText.setFont(font);
    restartText.setCharacterSize(25);
    restartText.setFillColor(sf::Color::White);
    restartText.setPosition(470.0f, 380.0f);
    restartText.setString("Press R to Restart");

    sf::Text quitText;
    quitText.setFont(font);
    quitText.setCharacterSize(25);
    quitText.setFillColor(sf::Color::White);
    quitText.setPosition(490.0f, 420.0f);
    quitText.setString("Press Q to Quit");

    std::vector<ecs::Entity> dynamicEntities;

    while (window.isOpen())
    {
        float dt = gameClock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
                return 0;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                return 0;
            }
        }

        if (gameOver)
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
            {
                window.close();
                return 0;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::R))
            {
                for (auto e : dynamicEntities)
                    ecs::destroy_entity(e);
                dynamicEntities.clear();

                auto& ph = ecs::get_component<Health>(player);
                auto& pp = ecs::get_component<Vector2d>(player);
                ph.hp = 5;
                pp.x = 608.0f;
                pp.y = 328.0f;

                score = 0;
                enemySpawnTimer = 0.0f;
                enemySpawnDelay = 2.0f;
                shootCooldown = 0.0f;
                invincibilityTimer = 0.0f;
                lastShootDir = Vector2d(0, -1);
                gameOver = false;
                continue;
            }

            window.clear(sf::Color::Black);
            window.draw(backgroundSprite);
            window.draw(gameOverText);
            scoreText.setString("Score: " + std::to_string(score));
            window.draw(scoreText);
            window.draw(restartText);
            window.draw(quitText);
            window.display();
            continue;
        }

        auto& playerHealth = ecs::get_component<Health>(player);
        auto& playerPos = ecs::get_component<Vector2d>(player);

        if (playerHealth.hp <= 0)
        {
            gameOver = true;
            continue;
        }

        invincibilityTimer -= dt;

        auto& playerMov = ecs::get_component<Movement>(player);
        if (playerMov.direction.x != 0.0f || playerMov.direction.y != 0.0f)
            lastShootDir = playerMov.direction;

        shootCooldown -= dt;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && shootCooldown <= 0.0f)
        {
            shootCooldown = 0.3f;

            ecs::Entity proj = ecs::create_entity();
            dynamicEntities.push_back(proj);
            ecs::add_components(proj,
                Vector2d(playerPos.x + 32.0f, playerPos.y + 40.0f),
                Movement(lastShootDir, 400.0f),
                ShapeRenderer(6.0f, sf::Color::Yellow),
                ProjectileTag{},
                Collider(6.0f)
            );
        }

        enemySpawnTimer += dt;
        if (enemySpawnTimer >= enemySpawnDelay)
        {
            enemySpawnTimer = 0.0f;

            int side = GenerateRandomInt(0, 3);
            float ex = 0.0f;
            float ey = 0.0f;

            if (side == 0)
            {
                ex = static_cast<float>(GenerateRandomInt(0, 1280));
                ey = -72.0f;
            }
            else if (side == 1)
            {
                ex = static_cast<float>(GenerateRandomInt(0, 1280));
                ey = 720.0f;
            }
            else if (side == 2)
            {
                ex = -72.0f;
                ey = static_cast<float>(GenerateRandomInt(0, 720));
            }
            else
            {
                ex = 1280.0f;
                ey = static_cast<float>(GenerateRandomInt(0, 720));
            }

            int textureIndex = GenerateRandomInt(0, 6);

            ecs::Entity enemy = ecs::create_entity();
            dynamicEntities.push_back(enemy);
            ecs::add_components(enemy,
                Vector2d(ex, ey),
                Movement(Vector2d(0, 0), 80.0f),
                SpriteRenderer(enemyTextures[textureIndex]),
                EnemyTag{},
                Collider(30.0f),
                ColliderOffset(36.0f, 36.0f)
            );

            auto& enemySprite = ecs::get_component<SpriteRenderer>(enemy);
            enemySprite.sprite.setScale(0.75f, 0.75f);

            if (enemySpawnDelay > 0.5f)
                enemySpawnDelay -= 0.05f;
        }

        inputSystem->update();
        enemyAISystem->update(playerPos);
        movementSystem->update(dt);
        animationSystem->update(dt);

        auto collisionResult = collisionSystem->update();

        if (collisionResult.playerHit && invincibilityTimer <= 0.0f)
        {
            playerHealth.hp--;
            invincibilityTimer = 1.0f;
        }

        score += static_cast<int>(collisionResult.destroyedEnemies.size());

        std::set<ecs::Entity> toDestroy;
        for (auto e : collisionResult.destroyedProjectiles)
            toDestroy.insert(e);
        for (auto e : collisionResult.destroyedEnemies)
            toDestroy.insert(e);
        for (auto e : toDestroy)
            ecs::destroy_entity(e);
        dynamicEntities.erase(
            std::remove_if(dynamicEntities.begin(), dynamicEntities.end(),
                [&toDestroy](ecs::Entity e) { return toDestroy.count(e) > 0; }),
            dynamicEntities.end());

        window.clear(sf::Color::Black);

        window.draw(backgroundSprite);
        shapeRenderSystem->render(window);
        spriteRenderSystem->render(window);

        scoreText.setString("Score: " + std::to_string(score));
        healthText.setString("HP: " + std::to_string(playerHealth.hp));
        window.draw(scoreText);
        window.draw(healthText);

        window.display();
    }

    return 0;
}

} // namespace game::main
