#include <SFML/Graphics.hpp>
#include <cmath>
#include <SFML/Audio.hpp>
#include <iostream>
#include "Bullet.h"
#include "lander.h"
#include "SpaceShip.h"
#include "humanoid.h"

// Collision detection logic
bool isCollision(const sf::FloatRect &rect1, const sf::FloatRect &rect2)
{
    return rect1.intersects(rect2);
}

int main()
{
    const int gameWidth = 1600; // The width of the game screen.
    const int gameHeight = 900; // The height of the game screen.

    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 50), "Defender Game", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60); // Helps reduce toll on processing.
    sf::Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("resources/Space-mountain.png"))
    {
        return EXIT_FAILURE;
    }

    sf::Sprite Background;
    Background.setTexture(BackgroundTexture);
    Background.setPosition(0, 0);
    Background.setScale(1.0f, 1.0f);

    // Setting up a game view to allow for scrolling.
    sf::View GameView;
    GameView.reset(sf::FloatRect(0, 0, gameWidth, gameHeight));

    // Setting up a MiniMap View.
    sf::View MiniMapView;
    MiniMapView.reset(sf::FloatRect(0.f, 0.f, Background.getGlobalBounds().width, Background.getGlobalBounds().height));
    MiniMapView.setViewport(sf::FloatRect(0.2f, 0.f, 0.66f, 0.15f));

    // Setting up a higlight square for the minimap. 
    sf::RectangleShape CurrentView(sf::Vector2f(gameWidth, gameHeight));
    CurrentView.setOrigin(gameWidth / 2, gameHeight / 2);
    CurrentView.setFillColor(sf::Color::Transparent);
    CurrentView.setOutlineThickness(-10);
    CurrentView.setOutlineColor(sf::Color::Red);

    sf::Texture spaceShipTexture;
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png"))
    {
        return EXIT_FAILURE;
    }

    // Create a sprite for the space ship and set its texture
    float scale = 2.5f;

    // Create the space ship.
    const int ShipSize = 175;
    sf::Vector2f spaceShipPosition((Background.getGlobalBounds().width / 2) - ShipSize, gameHeight / 2);
    const float shipSpeed = 1000.f;

    // Create a hitbox for the space ship
    sf::FloatRect spaceShipHitBox;

    sf::Clock bulletCooldownTimer;
    const sf::Time bulletCooldownDuration = sf::seconds(0.5f); // Adjust the cooldown duration as needed
    bool canShootBullet = true;

    auto landerShot = 0;

    SpaceShip spaceShip(scale, shipSpeed, spaceShipPosition, Background); // Create the spaceShip

    // Load the text font
    sf::Font font;
    if (!font.loadFromFile("resources/sansation.ttf"))
        return EXIT_FAILURE;

    // Initialise Game instructions
    sf::Text playInstructions;
    playInstructions.setFont(font);
    playInstructions.setCharacterSize(20);
    playInstructions.setPosition(gameWidth / 2 - 400, gameHeight / 2);
    playInstructions.setFillColor(sf::Color::White);
    playInstructions.setString("Use WASD to move and space to shoot");
    
    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(50);
    pauseMessage.setPosition(gameWidth / 2 - 400, gameHeight / 2);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("Welcome to Defender!\nPress A to start the game");

    // Define the space ship's properties.
    sf::Clock AITimer;
    const sf::Time AITime = sf::seconds(0.1f);
    sf::Clock clock;
    auto isPlaying = false;
    auto SetInstructionPosition = false;
    auto Pause = false;

    // Define a vector to store the landers
    std::vector<Lander> landers;
    std::vector<Explosion> explosions;

    std::vector<Humanoid> humanoids;

    // Define a timer for spawning landers
    sf::Clock landerSpawnTimer;
    float spawnInterval = 5.0f;

    auto isleft = true; // bool for the direction that the lazer shoots.

    std::vector<Bullet> bullets;
    auto bulletSpeed = 40.f;

    while (window.isOpen())
    {

        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
            if ((event.type == sf::Event::Closed) ||
                ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)))
            {
                window.close();
                break;
            }

            // Enter key pressed: play
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::A) && (!Pause))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();
                    float time = 0;
                    isleft = spaceShip.SpaceShipControl(time, bulletSpeed, gameWidth, gameHeight, bullets, GameView, window);

                    // Reset the position of space ship and clear alien objects
                    SpaceShip spaceShip(scale, shipSpeed, spaceShipPosition, Background);
                     humanoids.clear();
                     for (auto i=0; i<5; ++i){
                        auto landWidth = gameWidth * 5;
                        float xPosition = i * landWidth / 5;
                        float yPosition = gameHeight - 150;
                        int direction = (i % 2 == 0) ? 1 : -1; // Alternating direction

                        sf::Vector2f startPosition(xPosition, yPosition);
                        Humanoid newHumanoid(startPosition, direction, 1.0);
                        humanoids.push_back(newHumanoid);
                    }
                    landers.clear();
                    explosions.clear();
                    landerShot = 0;
                }
            }
            if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter) && (Pause))
            {
                if (!isPlaying)
                {
                    // (re)start the game
                    isPlaying = true;
                    clock.restart();
                    float time = 0;
                    isleft = spaceShip.SpaceShipControl(time, bulletSpeed, gameWidth, gameHeight, bullets, GameView, window);

                    // Reset the position of space ship and clear alien objects
                    // spaceShip.setPosition(spaceShipPosition);
                    SpaceShip spaceShip(scale, shipSpeed, spaceShipPosition, Background);
                    landers.clear();
                    explosions.clear();
                    landerShot = 0;
                }
            }
        }
        if (isPlaying)
        {
            float deltaTime = clock.restart().asSeconds();
            isleft = spaceShip.SpaceShipControl(deltaTime, bulletSpeed, gameWidth, gameHeight, bullets, GameView, window);

            // Check if it's time to spawn a new lander
            if (landerSpawnTimer.getElapsedTime().asSeconds() >= spawnInterval)
            {
                // Create a new lander at a random position around the spaceship
                // sf::Vector2f randomOffset;
                float distance = 600.0f; // Adjust the distance from the spaceship
                // float angle = static_cast<float>(std::rand() % 360); // Random angle in degrees
                // randomOffset.x = std::cos(angle * 3.14159265f / 180) * distance;
                // randomOffset.y = std::sin(angle * 3.14159265f / 180) * distance;

                // // Calculate the lander's position relative to the spaceship
                // sf::Vector2f landerPosition //= spaceShip.getPosition() + randomOffset;

                float y_min = gameHeight * 0.15;
                float y_max = gameHeight - 300.f;

                // Create the lander and add it to the vector
                Lander newLander(distance, spaceShip, y_min, y_max);
                landers.push_back(newLander);

                // Restart the spawn timer with a new random interval
                spawnInterval = 2.0f + static_cast<float>(std::rand()) / (RAND_MAX / 6.0f);
                landerSpawnTimer.restart();
            }

            if (isleft)
            {
                spaceShipHitBox.left = spaceShip.getPosition().x + 8 * scale;
                spaceShipHitBox.top = spaceShip.getPosition().y + 10 * scale;
                spaceShipHitBox.width = 20 * scale;
                spaceShipHitBox.height = 5 * scale;
            }
            else
            {
                spaceShipHitBox.left = spaceShip.getPosition().x - 24 * scale;
                spaceShipHitBox.top = spaceShip.getPosition().y + 10 * scale;
                spaceShipHitBox.width = 20 * scale;
                spaceShipHitBox.height = 10 * scale;
            }

            // Check collision with missiles
            for (auto &lander : landers)
            {
                for (size_t i = 0; i < lander.missiles.size(); ++i)
                {
                    if (spaceShipHitBox.intersects(lander.missiles[i].getHitBox()))
                    {
                        isPlaying = false;
                        SetInstructionPosition = true;
                        Pause = true;
                        pauseMessage.setString("Game Over!\nPress Enter to restart or\nescape to exit");
                        break; // Exit the loop early, as we already detected a collision
                    }
                }
            }

            for (auto &lander : landers)
            {
                sf::FloatRect landerHitBox = lander.getHitBox();
                if (isCollision(spaceShipHitBox, landerHitBox))
                {
                    isPlaying = false;
                    SetInstructionPosition = true;
                    Pause = true;
                    pauseMessage.setString("Game Over!\nPress Enter to restart or\nescape to exit");
                }
            }
            // Check collision between bullets and the lander
            for (auto &lander : landers)
            {
                for (auto &bullet : bullets)
                {
                    if (!bullet.isActive())
                        continue; // Skip inactive bullets

                    sf::FloatRect bulletHitBox = bullet.getHitBox();
                    if (lander.getHitBox().intersects(bulletHitBox))
                    {
                        // Bullet hit the lander
                        bullet.setActive(false); // Deactivate the bullet
                        lander.destroy();
                        landerShot++;

                        Explosion newExplosion(lander.getPosition(), 6, 0.005f);

                        // Restart the explosion animation
                        newExplosion.startAnimation();
                        explosions.push_back(newExplosion);

                        // Remove destroyed landers from the vector
                        landers.erase(std::remove_if(landers.begin(), landers.end(), [](const Lander &lander)
                                                     { return !lander.isActive(); }),
                                      landers.end());

                        // Optionally, you can handle other actions when the lander is destroyed

                        break; // Exit the loop early, as we only need to handle one collision
                    }

                    if (landerShot == 10)
                    {
                        isPlaying = false;
                        pauseMessage.setString("YOU WON!\nNow go get some fresh air!");
                        playInstructions.setString("Press Esc to exit the game.");
                        break;
                    }
                }
            }
        }

        for (auto &bullet : bullets)
        {
            bullet.update();
        }

        // rendering
        window.clear();
        if (isPlaying)
        {

            GameView.setCenter(spaceShip.getPosition().x, gameHeight / 2);

            // Draw objects in the main view.
            window.setView(GameView);
            Background.setColor(sf::Color(255, 255, 255, 255));
            window.draw(Background);
            spaceShip.draw(window);

            float deltaTime = clock.restart().asSeconds();
            Bullet::removeInactiveBullets(bullets);
            spaceShip.draw(window);

            for (auto &bullet : bullets)
            {
                bullet.draw(window);
            }

            for (auto& humanoid : humanoids){
                humanoid.updatePosition();
            }

            for (auto& humanoid : humanoids) {
                humanoid.draw(window);
            }

            for (auto &explosion : explosions)
            {
                explosion.update(deltaTime);
            }

            for (auto &explosion : explosions)
            {
                if (!explosion.isFinished())
                {
                    explosion.draw(window);
                }
            }

            // Call updateMissile to handle missile shooting
            for (auto &lander : landers)
            {
                auto landWidth = gameWidth * 5;
                lander.missileShoot(deltaTime, landWidth, gameHeight, spaceShip.getPosition());
                // Update and draw the enemy
                if (isleft)
                {
                    lander.updatePosition(spaceShip.getPosition() + sf::Vector2f(8 * scale, 0), deltaTime);
                }
                else
                {
                    lander.updatePosition(spaceShip.getPosition() - sf::Vector2f(16 * scale, 0), deltaTime);
                }

                lander.draw(window);
                lander.missileDraw(window);
            }

            // Drawing the minimap view.
            window.setView(MiniMapView);

            Background.setColor(sf::Color(255, 255, 255, 150));
            window.draw(Background);
            spaceShip.draw(window);
            CurrentView.setPosition(spaceShip.getPosition().x, gameHeight / 2); // Set the position of the highlighted view.
            window.draw(CurrentView);

            for (auto &bullet : bullets)
            {
                bullet.draw(window);
            }

            for (auto& humanoid : humanoids){
                humanoid.updatePosition();
            }

            for (auto& humanoid : humanoids) {
                humanoid.draw(window);
            }

            for (auto &explosion : explosions)
            {
                explosion.update(deltaTime);
            }

            for (auto &explosion : explosions)
            {
                if (!explosion.isFinished())
                {
                    explosion.draw(window);
                }
            }

            // Call updateMissile to handle missile shooting
            for (auto &lander : landers)
            {            
                auto landWidth = gameWidth * 5;
                lander.missileShoot(deltaTime, landWidth, gameHeight, spaceShip.getPosition());
                // Update and draw the enemy
                if (isleft)
                {
                    lander.updatePosition(spaceShip.getPosition() + sf::Vector2f(8 * scale, 0), deltaTime);
                }
                else
                {
                    lander.updatePosition(spaceShip.getPosition() - sf::Vector2f(16 * scale, 0), deltaTime);
                }

                lander.draw(window);
                lander.missileDraw(window);
            }
        }
        else
        {
            GameView.setCenter(spaceShip.getPosition().x, gameHeight / 2);
            window.setView(GameView);
            window.draw(Background);
            spaceShip.draw(window);
            // Draw the pause message
            if (SetInstructionPosition)
            {
                playInstructions.setPosition(spaceShip.getPosition().x - gameWidth / 3, gameHeight / 4 + 250);
                pauseMessage.setPosition(spaceShip.getPosition().x - gameWidth / 3, gameHeight / 3);
            }
            else
            {
                playInstructions.setPosition(spaceShip.getPosition().x - gameWidth / 3, gameHeight / 4 + 120);
                pauseMessage.setPosition(spaceShip.getPosition().x - gameWidth / 3, gameHeight / 4);
            }

            window.draw(playInstructions);
            window.draw(pauseMessage);
        }

        window.display();
    }
}