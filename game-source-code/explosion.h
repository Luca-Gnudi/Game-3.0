#ifndef EXPLOSION
#define EXPLOSION

/** \file explosion.h
    \brief Contains code for constructing the explosions

 */

#include <SFML/Graphics.hpp>

class Explosion {
public:
    Explosion(sf::Vector2f position, int totalFrames, float frameDuration);
    void setPosition(sf::Vector2f position);
    void startAnimation();
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    bool isFinished() const;

private:
    sf::Sprite explosionSprite;
    sf::Texture *explosionTexture;
    int currentFrame;
    int totalFrames;
    float frameDuration;
    float frameTimer;
    bool finished;
};

#endif