#include "missile.h"

Missile::Missile(sf::Vector2f position, sf::Vector2f targetPosition) {
    missileTexture = new sf::Texture;
    if (!missileTexture->loadFromFile("resources/assets/missile.png")) {
        std::cout << "Could not load missile image file" << std::endl;
    }
    missileSprite.setTexture(*missileTexture);
    missileSprite.setScale(2.0f, 2.0f);
    missileSprite.setPosition(position);
    speed = 10000.0f; // Adjust the speed as needed

     // Calculate the direction vector towards the target (space ship)
    missileDirection = targetPosition - position;

    // Normalize the direction vector (make it a unit vector)
    float length = std::sqrt(missileDirection.x * missileDirection.x + missileDirection.y * missileDirection.y);
    if (length != 0) {
        missileDirection /= length;
    }
}

sf::FloatRect Missile::getHitBox() {
    sf::FloatRect missileHitBox;
    auto scale = 2.0f;

    missileHitBox.left = missileSprite.getPosition().x + 12*scale;
    missileHitBox.top = missileSprite.getPosition().y + 14*scale;
    missileHitBox.width = 9*scale;
    missileHitBox.height = 9*scale;

    return missileHitBox;
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
    return position.x > gameWidth * 5 || position.x < 0 || position.y > gameHeight || position.y < 0;
}