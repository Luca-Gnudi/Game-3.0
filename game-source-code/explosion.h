#ifndef EXPLOSION
#define EXPLOSION

/** \file explosion.h
    \brief Contains code for constructing the explosions

 */

#include <SFML/Graphics.hpp>

class Explosion {
public:
    Explosion(sf::Texture& texture, sf::Vector2f position, int totalFrames, float frameDuration);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isFinished() const;

private:
    sf::Sprite explosionSprite;
    int currentFrame;
    int totalFrames;
    float frameDuration;
    float frameTimer;
    bool finished;
};

#endif