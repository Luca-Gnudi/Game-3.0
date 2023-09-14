#ifndef LANDER
#define LANDER

/** \file lander.h
    \brief Contains code for constructing the landers
 */

#include <SFML/Graphics.hpp>

class Lander {
public:
    Lander(sf::Vector2f startPosition); // Constructor
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite landerSprite;
    sf::Texture landerTexture;
    float speed;
};

#endif