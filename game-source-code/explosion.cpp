#include "explosion.h"
#include <iostream>

Explosion::Explosion(sf::Vector2f position = sf::Vector2f(0, 0), int totalFrames = 1, float frameDuration = 0.1f)
    : totalFrames(totalFrames), frameDuration(frameDuration), currentFrame(0), frameTimer(0), finished(false) {
    explosionTexture = new sf::Texture;
    if (!explosionTexture->loadFromFile("resources/assets/explosion.png")) {
        std::cout << "Could not load explosion sprite sheet file";
    }
    explosionSprite.setTexture(*explosionTexture);
    explosionSprite.setScale(5.5f,5.5f);
    explosionSprite.setPosition(position);
}

void Explosion::setPosition(sf::Vector2f position) {
    explosionSprite.setPosition(position);
}

void Explosion::startAnimation() {
    currentFrame = 0;
    frameTimer = 0;
    finished = false;
}

void Explosion::update(float deltaTime) {
    if (!finished) {
        frameTimer += deltaTime;
        if (frameTimer >= frameDuration) {
            // Move to the next frame
            currentFrame++;
            frameTimer = 0;

            if (currentFrame >= totalFrames) {
                // Animation finished
                finished = true;
            }
        }
    }
}

void Explosion::draw(sf::RenderWindow& window) {
    if (!finished) {
        // Calculate the width of each frame in the sprite sheet
        int frameWidth = explosionSprite.getTexture()->getSize().x / totalFrames;

        // Set the texture rect to display the current frame
        explosionSprite.setTextureRect(sf::IntRect(currentFrame * frameWidth, 0, frameWidth, explosionSprite.getTexture()->getSize().y));

        // Draw the explosion sprite
        window.draw(explosionSprite);
    }
}

bool Explosion::isFinished() const {
    return finished;
}