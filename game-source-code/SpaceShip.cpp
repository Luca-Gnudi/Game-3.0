#include "SpaceShip.h"
#include <iostream>

SpaceShip::SpaceShip(const float& scale, const float& speed, sf::Vector2f& StartPosition, sf::Sprite& ground, sf::Texture GroundTexture){
    //Initialise the lazer sprite.
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png")) {
        std::cout << "Failed to load SpaceShip image";
    }

    spaceShip.setTexture(spaceShipTexture);
    spaceShip.setScale(ShipScale,ShipScale);
    initialPosition = StartPosition;
    spaceShip.setPosition(initialPosition);
    spaceShip.setOrigin(ShipScale,ShipScale);
    ShipSpeed = speed;
    ShipScale = scale;
    Ground = ground;
    
    //Create an inverted copy of the ground for screen scrolling.
    groundScale = ground.getScale();
    Groundinverted.setTexture(GroundTexture);
    Groundinverted.setScale(-2.5f,2.5f);

    //Initialise lazerSound to be played when bullets shoot.
    sf::SoundBuffer LazerSoundBuffer;
    if(!LazerSoundBuffer.loadFromFile("resources/blaster-2-81267.mp3"))
	    std::cout << "Cannot load lazer sound file";
    sf::Sound LazerSound(LazerSoundBuffer);
}

void SpaceShip::draw(sf::RenderWindow& window){
    window.draw(spaceShip);
}

sf::Vector2f SpaceShip::getPosition() const{
    return spaceShip.getPosition();
}

void SpaceShip::SpaceShipControl(const float& deltaTime, const float& bulletSpeed, const int& gameWidth, const int& gameHeight, std::vector<Bullet>& bullets){
     // Moving the space ship vertically and horizontally
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (spaceShip.getPosition().y > 0)) {
                spaceShip.move(0.f, -ShipSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (spaceShip.getPosition().y + spaceShip.getLocalBounds().height < gameHeight-100)) {
                spaceShip.move(0.f, ShipSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if ((spaceShip.getPosition().x < gameWidth/6 )){
                    Ground.move(ShipSpeed * deltaTime, 0.f);
                    Groundinverted.move(-ShipSpeed*deltaTime,0.f);
                    //Puts inverted ground next to actual ground.
                    if ((Ground.getGlobalBounds().width < 0) && (!isGroundLeftCopy)){
                        Groundinverted.setPosition(Groundinverted.getGlobalBounds().width,0);
                        std::cout << "Position is set" << std::endl;
                        isGroundLeftCopy = true;
                        isGroundRightCopy = false;//Resets the other copy.
                    }
                }
                else{
                    spaceShip.move(-ShipSpeed * deltaTime, 0.f);
                    spaceShip.setScale(ShipScale,ShipScale);
                }
                isleft = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                if((spaceShip.getPosition().x + spaceShip.getLocalBounds().width > gameWidth-gameWidth/6)){
                    Ground.move(-ShipSpeed * deltaTime, 0.f);
                    Groundinverted.move(-ShipSpeed*deltaTime,0.f);
                    //Puts inverted ground next to actual ground.
                    if ((Ground.getGlobalBounds().width > gameWidth) && (!isGroundRightCopy)){
                        Groundinverted.setPosition(0,0);
                        std::cout << "Position is set" << std::endl;
                        isGroundRightCopy = true;
                        isGroundLeftCopy = false;//Resets the other copy.
                    }

                }
                else{
                    spaceShip.move(ShipSpeed * deltaTime, 0.f);
                    spaceShip.setScale(-ShipScale,ShipScale);
                }
                isleft = false;
            }
            //Shooting lazers.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
                if (isleft){
                    Bullet bullet(spaceShip.getPosition(), -1, bulletSpeed);
                    bullet.setActive(true);
                    bullets.push_back(bullet);
                }
                else {
                    Bullet bullet(spaceShip.getPosition(), 1, bulletSpeed);
                    bullet.setActive(true);
                    bullets.push_back(bullet);
                }
                LazerSound.play();
            }
}

//Could put inside the draw construstor of the SpaceShip.
void SpaceShip::GroundDraw(sf::RenderWindow& window){
    window.draw(Ground);
    window.draw(Groundinverted);
}

