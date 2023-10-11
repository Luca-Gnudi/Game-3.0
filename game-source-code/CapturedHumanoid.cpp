#include "capturedhumanoid.h"

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
    if (active){
    // Adjust the speed as needed
    float moveSpeed = 2000.0f;
    capturedhumanoidSprite.move(0.0f, moveSpeed * deltaTime);
    }
}

sf::FloatRect CapturedHumanoid::getHitBox() {
    capturedHumanoidHitBox.left = capturedhumanoidSprite.getPosition().x + 10.0f;
    capturedHumanoidHitBox.top = capturedhumanoidSprite.getPosition().y + 20.0;
    capturedHumanoidHitBox.width = 40.0;
    capturedHumanoidHitBox.height = 60.0;
    return capturedHumanoidHitBox;
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