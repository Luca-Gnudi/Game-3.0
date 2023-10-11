#ifndef CAPTUREDHUMANOID
#define CAPTUREDHUMANOID

/** \file capturedhumanoid.h
    \brief Contains code for constructing the humanoids that are captured and being carried

 */

#include <SFML/Graphics.hpp>
#include <iostream>

class CapturedHumanoid {
public:
    CapturedHumanoid();
    sf::FloatRect getHitBox();
    void setPosition(const sf::Vector2f& position);
    void updatePosition(float deltaTime);
    void draw(sf::RenderWindow& window);
    void setActive(bool isActive);
    bool isActive() const;
    sf::Vector2f getPosition() const;
    float getHeight() const;
  

private:
    sf::Sprite capturedhumanoidSprite;
    sf::Texture *capturedhumanoidTexture;
    sf::FloatRect capturedHumanoidHitBox;

    float velocity;
    int direction;
    bool active;

    
};

#endif