#include "lander.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "missile.h"

Lander::Lander(sf::Vector2f startPosition) {
    if (!landerTexture.loadFromFile("resources/assets/lander.png")) {
        std::cout << "Could not load lander image file";
    }
    landerSprite.setTexture(landerTexture);
    landerSprite.setScale(5.5f, 5.5f); // Adjust the scale as needed
    landerSprite.setPosition(startPosition);
    speed = 200.0f; // Adjust the speed as needed

    // Initialize the missiles vector
    missiles = std::vector<Missile>();

    missileTimer.restart(); // Start the timer
    minDelay = 3.0f; // Minimum delay in seconds
    maxDelay = 10.0f; // Maximum delay in seconds
    randomDelay = minDelay + static_cast<float>(std::rand()) / (RAND_MAX / (maxDelay - minDelay));
}

void Lander::updatePosition(sf::Vector2f spaceshipPosition, float deltaTime) {
     // Calculate the direction vector from lander to spaceship
    sf::Vector2f direction = spaceshipPosition - landerSprite.getPosition();

    // Normalize the direction vector (make it a unit vector)
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length != 0) {
        direction /= length;
    }

    // Set the speed at which the lander moves
    float moveSpeed = 10000.0f; // Adjust the speed as needed

    // Update the lander's position based on the direction and speed
    landerSprite.move(direction * moveSpeed * deltaTime);
}

sf::Vector2f Lander::getPosition() const {
    return landerSprite.getPosition();
}

sf::FloatRect Lander::getHitBox() {
    sf::FloatRect landerHitBox;
    auto scale = 5.5f;

    landerHitBox.left = landerSprite.getPosition().x + 5*scale;
    landerHitBox.top = landerSprite.getPosition().y + 7*scale;
    landerHitBox.width = 24*scale;
    landerHitBox.height = 22*scale;

    return landerHitBox;
}

void Lander::draw(sf::RenderWindow& window) {
    window.draw(landerSprite);
}

void Lander::missileCreate(sf::Vector2f spaceshipPosition) {
    Missile missile(landerSprite.getPosition(), spaceshipPosition);
    missiles.push_back(missile);
}

void Lander::missileShoot(float deltaTime, int gameWidth, int gameHeight, sf::Vector2f spaceshipPosition ) {
   if (missileTimer.getElapsedTime().asSeconds() >= randomDelay) {

        // Create a missile with the calculated direction
        Missile missile(landerSprite.getPosition(), spaceshipPosition);
        missiles.push_back(missile);

        missileTimer.restart(); // Reset the timer
        // Generate a new random shooting delay
        randomDelay = minDelay + static_cast<float>(std::rand()) / (RAND_MAX / (maxDelay - minDelay));
    }

    // Update existing missiles
    missileUpdate(deltaTime, gameWidth, gameHeight);
}

void Lander::missileUpdate(float deltaTime, int gameWidth, int gameHeight) {
    for (size_t i = 0; i < missiles.size(); ++i) {
        missiles[i].updatePosition(deltaTime);
        if (missiles[i].isOutOfBounds(gameWidth, gameHeight)) {
            // Remove missiles that are out of bounds
            missiles.erase(missiles.begin() + i);
            --i;
        }
    }
}

void Lander::missileDraw(sf::RenderWindow& window) {
    for (Missile& missile : missiles) {
        missile.draw(window);
    }
}
