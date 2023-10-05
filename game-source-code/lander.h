#ifndef LANDER
#define LANDER

/** \file lander.h
    \brief Contains code for constructing the landers

 */

#include <SFML/Graphics.hpp>
#include <vector>
#include "missile.h"
#include "explosion.h"
#include "SpaceShip.h"
#include "CapturedHumanoid.h"

class Lander {
public:

    Lander(); // Constructor
    sf::FloatRect getHitBox();
    void updatePosition(sf::Vector2f spaceshipPosition, float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2f getPosition() const;

    std::vector<Missile> missiles;
    
    void missileCreate(sf::Vector2f spaceshipPosition);
    void missileUpdate(float deltaTime, int gameWidth, int gameHeight);
    void missileShoot(float deltaTime, int gameWidth, int gameHeight, sf::Vector2f spaceshipPosition);
    void missileDraw(sf::RenderWindow& window);
    //sf::Vector2f spawn(const float& distance, sf::Sprite& spaceShip);
    void destroy();
    bool isDestroyed() const;
    bool isActive() const;

    void captureHumanoid(const Humanoid& humanoid);
    void moveWithHumanoid(float deltaTime);

    bool isCarryingHumanoid;
    
private:
    sf::Sprite landerSprite;
    sf::Texture *landerTexture;
    float speed;

    sf::Clock missileTimer;
    float minDelay;
    float maxDelay;
    float randomDelay;
    sf::Vector2f landerPosition;

    bool destroyed;
    Explosion explosion;

    sf::Vector2f capturedHumanoidPosition;
    CapturedHumanoid capturedHumanoid;
};

#endif