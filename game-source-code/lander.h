#ifndef LANDER
#define LANDER

/** \file lander.h
    \brief Contains code for constructing the landers
 */

#include <SFML/Graphics.hpp>

class Lander {
public:
    Lander(sf::Vector2f startPosition); // Constructor
    void updatePosition(sf::Vector2f spaceshipPosition, float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

private:
    sf::Sprite landerSprite;
    sf::Texture landerTexture;
    float speed;
};

#endif