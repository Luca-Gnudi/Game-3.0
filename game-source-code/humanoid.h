#ifndef HUMANOID
#define HUMANOID

/** \file humanoid.h
    \brief Contains code for constructing the humanoids

 */

#include <SFML/Graphics.hpp>

class Humanoid {
public:
    Humanoid(sf::Vector2f, int dir, float velocity);
    sf::FloatRect getHitBox();
    void updatePosition(sf::Vector2f humanoidPosition, float deltaTime);
    void draw(sf::RenderWindow& window);
    void setActive(bool isActive);
    bool isActive() const;
    sf::Vector2f getPosition() const;

private:
    sf::Sprite humanoidSprite;
    sf::Texture *humanoidTexture;

    float velocity;
    int direction;
    bool active;
};

#endif