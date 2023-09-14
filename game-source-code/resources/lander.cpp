#include "lander.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Lander::Lander(sf::Vector2f startPosition) {
    if (!landerTexture.loadFromFile("assets/lander.png")) {
        std::cout << "Could not load lander image file";
    }
    landerSprite.setTexture(landerTexture);
    landerSprite.setScale(5.5f, 5.5f); // Adjust the scale as needed
    landerSprite.setPosition(startPosition);
    speed = 200.0f; // Adjust the speed as needed
}

void Lander::update(float deltaTime) {
    // Implement enemy movement logic here

}

void Lander::draw(sf::RenderWindow& window) {
    window.draw(landerSprite);
}