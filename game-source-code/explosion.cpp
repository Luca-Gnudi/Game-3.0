#include "explosion.h"

Explosion::Explosion(sf::Texture& texture, sf::Vector2f position, int totalFrames, float frameDuration)
    : explosionSprite(texture), totalFrames(totalFrames), frameDuration(frameDuration), currentFrame(0), frameTimer(0), finished(false) {
    explosionSprite.setPosition(position);
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