#include "lander.h"
#include <SFML/Graphics.hpp>
#include "lander.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>
#include <vector>
#include "missile.h"
#include "explosion.h"
#include "humanoid.h"

Lander::Lander(const float& distance, SpaceShip& spaceShip, const float& y_min, const float& y_max) 
: Entity(sf::Vector2f(0.f, 0.f)), destroyed(false), explosion(landerPosition, 6, 0.1f) {
    landerTexture = new sf::Texture;
    if (!landerTexture->loadFromFile("resources/assets/lander.png")) {
        std::cout << "Could not load lander image file";
    }
    landerSprite.setTexture(*landerTexture);
    landerSprite.setScale(2.5f, 2.5f); // Adjust the scale as needed

    //landerPosition = sf::Vector2f(1700.f,1000.f);

   // while ((landerPosition.y > y_min) && (landerPosition.y < y_max)){
        sf::Vector2f randomOffset;
        float angle = static_cast<float>(std::rand() % 360); // Random angle in degrees
        randomOffset.x = std::cos(angle * 3.14159265f / 180) * distance;
        randomOffset.y = std::sin(angle * 3.14159265f / 180) * distance;

        // Calculate the lander's position relative to the spaceship
        sf::Vector2f landerPosition = spaceShip.getPosition() + randomOffset;

   // }
    landerSprite.setPosition(landerPosition);
    speed = 200.0f; // Adjust the speed as needed

    // Initialize the missiles vector
    missiles = std::vector<Missile>();
    missileTimer.restart(); // Start the timer
    minDelay = 3.0f; // Minimum delay in seconds
    maxDelay = 10.0f; // Maximum delay in seconds
    randomDelay = minDelay + static_cast<float>(std::rand()) / (RAND_MAX / (maxDelay - minDelay));
}

void Lander::updatePosition(std::vector<Humanoid>& humanoids, float deltaTime) {
    // Check if there are any active humanoids
    bool foundHumanoid = false;
    sf::Vector2f closestHumanoidPosition;
    float closestHumanoidDistance = std::numeric_limits<float>::max();

    for (const Humanoid& humanoid : humanoids) {
        if (humanoid.isActive()) {
            // Calculate the direction vector from the lander to the humanoid
            sf::Vector2f direction = humanoid.getPosition() - landerSprite.getPosition();
            float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

            if (distance < closestHumanoidDistance) {
                closestHumanoidDistance = distance;
                closestHumanoidPosition = humanoid.getPosition();
                foundHumanoid = true;
            }
        }
    }

    // If an active humanoid is found, move towards it
    if (foundHumanoid) {
        sf::Vector2f direction = closestHumanoidPosition - landerSprite.getPosition();
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (length != 0) {
            direction /= length;
        }
        
        float moveSpeed = 1000.0f; // Adjust the speed as needed
        landerSprite.move(direction * moveSpeed * deltaTime);
    }
}

sf::Vector2f Lander::getPosition() const {
    return landerSprite.getPosition();
}

sf::FloatRect Lander::getHitBox() {
    sf::FloatRect landerHitBox;
    auto scale = 2.5f;

    landerHitBox.left = landerSprite.getPosition().x + 5*scale;
    landerHitBox.top = landerSprite.getPosition().y + 7*scale;
    landerHitBox.width = 24*scale;
    landerHitBox.height = 22*scale;

    return landerHitBox;
}

void Lander::draw(sf::RenderWindow& window) {

    if (!destroyed){
       window.draw(landerSprite);
    }
    else {
        // Draw explosion animation
        explosion.draw(window);

        // Check if the explosion animation has finished, and perform any cleanup
        if (explosion.isFinished()) {
            // Remove the lander or perform other cleanup actions as needed
        }
    }
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

void Lander::destroy() {
    destroyed = true;
    explosion.setPosition(landerSprite.getPosition());
    explosion.startAnimation();
}

bool Lander::isDestroyed() const {
    return destroyed;
}

bool Lander::isActive() const {
    return !destroyed;
}

void Lander::handleCollision(Entity& otherEntity) {
    // Check if the other entity is a humanoid
    Humanoid* humanoid = dynamic_cast<Humanoid*>(&otherEntity);
    if (humanoid) {
        // Pick up the humanoid and move up together
        moveUp(100.0f); // Adjust the distance as needed
        humanoid->setActive(false); // Deactivate the humanoid
    }
}