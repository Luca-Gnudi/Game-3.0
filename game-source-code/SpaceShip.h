#ifndef SPACESHIP_H
#define SPACESHIP_H
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTarget.hpp>
#include "Bullet.h"
#include <vector>
#include <iostream>

class SpaceShip{
public:
    SpaceShip(const float& scale, const float& speed, sf::Vector2f& StartPosition, sf::Sprite& Background);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;
    sf::FloatRect getHitBox();
    bool SpaceShipControl(const float& deltaTime, const float& bulletSpeed, const int& gameWidth, const int& gameHeight, std::vector<Bullet>& bullets, sf::View& GameView, sf::RenderWindow& window);
    void GroundDraw(sf::RenderWindow& window);

private:
    sf::Sprite spaceShip;
    sf::Vector2f initialPosition;
    float ShipSpeed;
    float ShipScale;
    sf::FloatRect spaceShipHitBox;
    bool isleft = true;
    //bool moveSpaceShip = false; //bool to show if the view or the spaceShip should be moved.
    sf::Sprite BackGround;
    sf::Texture spaceShipTexture;
    sf::Clock bulletCooldownTimer;
    sf::Time bulletCooldownDuration;

};

#endif