#ifndef SPACESHIP_H
#define SPACESHIP_H
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Bullet.h"
#include <vector>

class SpaceShip{
public:
    SpaceShip(const float& scale, const float& speed, sf::Vector2f& StartPosition, sf::Sprite& ground, sf::Texture GroundTexture);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    void SpaceShipControl(const float& deltaTime, const float& bulletSpeed, const int& gameWidth, const int& gameHeight, std::vector<Bullet>& bullets);
    void GroundDraw(sf::RenderWindow& window);

private:
    sf::Sprite spaceShip;
    sf::Vector2f initialPosition;
    float ShipSpeed;
    float ShipScale;
    sf::Sound LazerSound;
    bool isleft = true;
    sf::Sprite Ground;
    sf::Sprite Groundinverted;
    sf::Vector2f groundScale;
    //std::vector<Bullet> bullets;
    sf::Texture spaceShipTexture;
    bool isGroundLeftCopy = false;
    bool isGroundRightCopy = false;
};

#endif