#include "humanoid.h"
#include <iostream>

Humanoid::Humanoid( sf::Vector2f startPosition, int dir, float velocity)  
    : Entity(startPosition), velocity(velocity), direction(dir), active(true){
    humanoidTexture = new sf::Texture;
    if (!humanoidTexture->loadFromFile("resources/assets/Humanoid.png")) {
        std::cout << "Could not load lander image file";
    }
    humanoidSprite.setTexture(*humanoidTexture);
    humanoidSprite.setScale(1.0f, 1.0f); // Adjust the scale as needed
    humanoidSprite.setPosition(startPosition);
}

void Humanoid::updatePosition(){
    if (active) {
        humanoidSprite.move(velocity * direction, 0);

        if (humanoidSprite.getPosition().x < 750){
           direction = 1;
        }
        else if (humanoidSprite.getPosition().x + 60 > 5500){
           direction = -1;
        }

        if (direction == 1){
            humanoidSprite.setScale(-1.0f, 1.0f);
        }
        else if (direction == -1){
            humanoidSprite.setScale(1.0f, 1.0f);
        }
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