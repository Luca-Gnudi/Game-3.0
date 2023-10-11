#ifndef MISSILE
#define MISSILE

/** \file missile.h
    \brief Contains code for constructing the missiles

 */

#include <SFML/Graphics.hpp>
#include <iostream>
#include <math.h>

class Missile {
public:

    sf::Vector2f getPosition() const;
    Missile(sf::Vector2f position, sf::Vector2f target);
    sf::FloatRect getHitBox();
    void updatePosition(float deltaTime);
    sf::Vector2f getDirection() const;
    void draw(sf::RenderWindow& window);
    bool isOutOfBounds(int gameWidth, int gameHeight);

private:
    sf::Sprite missileSprite;
    sf::Texture *missileTexture;
    sf::Vector2f missileDirection;
    float speed;
};

#endif