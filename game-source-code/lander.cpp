#include "lander.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

Lander::Lander(sf::Vector2f startPosition) {
    if (!landerTexture.loadFromFile("assets/lander.png")) {
        std::cout << "Could not load lander image file";
    }
    landerSprite.setTexture(landerTexture);
    landerSprite.setScale(5.5f, 5.5f); // Adjust the scale as needed
    landerSprite.setPosition(startPosition);
    speed = 200.0f; // Adjust the speed as needed
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

void Lander::draw(sf::RenderWindow& window) {
    window.draw(landerSprite);
}