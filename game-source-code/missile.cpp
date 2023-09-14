#include "missile.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

Missile::Missile(sf::Vector2f position, sf::Vector2f targetPosition) {
    missileTexture = new sf::Texture;
    if (!missileTexture->loadFromFile("resources/assets/missile.png")) {
        std::cout << "Could not load missile image file" << std::endl;
    }
    missileSprite.setTexture(*missileTexture);
    missileSprite.setScale(5.0f, 5.0f);
    missileSprite.setPosition(position);
    speed = 20000.0f; // Adjust the speed as needed

     // Calculate the direction vector towards the target (space ship)
    missileDirection = targetPosition - position;

    // Normalize the direction vector (make it a unit vector)
    float length = std::sqrt(missileDirection.x * missileDirection.x + missileDirection.y * missileDirection.y);
    if (length != 0) {
        missileDirection /= length;
    }
}

sf::FloatRect Missile::getHitBox() {
    return missileSprite.getGlobalBounds();
}

void Missile::updatePosition(float deltaTime) {
    //Move the missile along the direction vector
    missileSprite.move(missileDirection * speed * deltaTime); 
}

sf::Vector2f Missile::getPosition() const {
    return missileSprite.getPosition();
}

sf::Vector2f Missile::getDirection() const {
    return missileDirection;
}

void Missile::draw(sf::RenderWindow& window) {
    window.draw(missileSprite);
}

bool Missile::isOutOfBounds(int gameWidth, int gameHeight) {
    // Check if the missile is out of bounds
    sf::Vector2f position = missileSprite.getPosition();
    return position.x > gameWidth || position.x < 0 || position.y > gameHeight || position.y < 0;
}