#include "humanoid.h"
#include <iostream>

Humanoid::Humanoid( sf::Vector2f startPosition, int dir, float velocity)  
    : velocity(velocity), direction(dir), active(true){
    humanoidTexture = new sf::Texture;
    if (!humanoidTexture->loadFromFile("resources/assets/Humanoid.png")) {
        std::cout << "Could not load lander image file";
    }
    humanoidSprite.setTexture(*humanoidTexture);
    humanoidSprite.setScale(1.5f, 1.5f); // Adjust the scale as needed
    humanoidSprite.setPosition(startPosition);
}

void Humanoid::updatePosition(sf::Vector2f humanoidPosition, float deltaTime){
    if (active) {
        humanoidSprite.move(velocity * direction, 0);
    }
}

sf::FloatRect Humanoid::getHitBox() {
    return humanoidSprite.getGlobalBounds();
}

void Humanoid::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(humanoidSprite);
    }
}

void Humanoid::setActive(bool isActive) {
    active = isActive;
}

bool Humanoid::isActive() const {
    return active;
}

sf::Vector2f Humanoid::getPosition() const{
    return humanoidSprite.getPosition();
}