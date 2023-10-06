#include "capturedhumanoid.h"
#include <iostream>

CapturedHumanoid::CapturedHumanoid()  {
    capturedhumanoidTexture = new sf::Texture;
    if (!capturedhumanoidTexture->loadFromFile("resources/assets/CapturedHumanoid.png")) {
        std::cout << "Could not load captured humanoid image file";
    }
    capturedhumanoidSprite.setTexture(*capturedhumanoidTexture);
    capturedhumanoidSprite.setScale(1.0f, 1.0f); // Adjust the scale as needed
}

void CapturedHumanoid::setPosition(const sf::Vector2f& position) {
    capturedhumanoidSprite.setPosition(position);
}

void CapturedHumanoid::updatePosition(float deltaTime) {
    if(active){
    // Adjust the speed as needed
    float moveSpeed = 100.0f;
    capturedhumanoidSprite.move(0.0f, moveSpeed * deltaTime);
    }
}

sf::FloatRect CapturedHumanoid::getHitBox() {
    return capturedhumanoidSprite.getGlobalBounds();
}

void CapturedHumanoid::draw(sf::RenderWindow& window) {
    window.draw(capturedhumanoidSprite);
}

void CapturedHumanoid::setActive(bool isActive) {
    active = isActive;
}

bool CapturedHumanoid::isActive() const {
    return active;
}

sf::Vector2f CapturedHumanoid::getPosition() const {
    return capturedhumanoidSprite.getPosition();
}

float CapturedHumanoid::getHeight() const {
    return capturedhumanoidSprite.getGlobalBounds().height;
}