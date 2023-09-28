#include "SpaceShip.h"
#include <iostream>
#include <SFML/Graphics/RenderTarget.hpp>

SpaceShip::SpaceShip(const float& scale, const float& speed, sf::Vector2f& StartPosition, sf::Sprite& Background){
    //Initialise the lazer sprite.
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png")) {
        std::cout << "Failed to load SpaceShip image";
    }

    spaceShip.setTexture(spaceShipTexture);
    spaceShip.setScale(ShipScale,ShipScale);
    spaceShip.setPosition(StartPosition);
    spaceShip.setOrigin(ShipScale,ShipScale);
    ShipSpeed = speed;
    ShipScale = scale;
    BackGround = Background;

    //Initialise lazerSound to be played when bullets shoot.
    //sf::SoundBuffer LazerSoundBuffer;
    //if(!LazerSoundBuffer.loadFromFile("resources/blaster-2-81267.mp3"))
	//    std::cout << "Cannot load lazer sound file";
    //sf::Sound LazerSound(LazerSoundBuffer);
} 


void SpaceShip::draw(sf::RenderWindow& window){
    window.draw(spaceShip);
}

sf::Vector2f SpaceShip::getPosition() const{
    return spaceShip.getPosition();
}

bool SpaceShip::SpaceShipControl(const float& deltaTime, const float& bulletSpeed, const int& gameWidth, const int& gameHeight, std::vector<Bullet>& bullets, sf::View& GameView, sf::RenderWindow& window){
     // Moving the space ship vertically and horizontally
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (spaceShip.getPosition().y > gameHeight*0.15)) {
                spaceShip.move(0.f, -ShipSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (spaceShip.getPosition().y + spaceShip.getLocalBounds().height < gameHeight-270)) {
                spaceShip.move(0.f, ShipSpeed * deltaTime);
            }
            //Moves the view and the ship if the ship gets close to the edge of the window.
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                if((window.mapCoordsToPixel(spaceShip.getPosition()).x < gameWidth/5) && (spaceShip.getPosition().x > 350)){
                    GameView.move(-ShipSpeed * deltaTime, 0.f);
                    spaceShip.move(-ShipSpeed * deltaTime, 0.f);
                    spaceShip.setScale(ShipScale,ShipScale);
                }
                //Moves just the ship if near the middle of the window
                else if(spaceShip.getPosition().x > gameWidth/2){
                    spaceShip.move(-ShipSpeed * deltaTime, 0.f);
                    spaceShip.setScale(ShipScale,ShipScale);
                }
                // else{// Makes the ship go to the other side of the map when it reaches the end.
                //     spaceShip.setPosition(BackGround.getGlobalBounds().width, spaceShip.getPosition().y);
                //     GameView.move(BackGround.getGlobalBounds().width - gameWidth-50, 0.f);
                // }
                isleft = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                //Moves the view and the ship if the ship gets close to the edge of the window.
                if((window.mapCoordsToPixel(spaceShip.getPosition()).x - spaceShip.getLocalBounds().width > gameWidth-gameWidth/5+70) && (spaceShip.getPosition().x < BackGround.getGlobalBounds().width - gameWidth/6)){
                    GameView.move(ShipSpeed * deltaTime, 0.f);
                    spaceShip.move(ShipSpeed*deltaTime, 0.f);
                    spaceShip.setScale(-ShipScale,ShipScale);
                    }
                //Moves just the ship if near the middle of the window
                else if (spaceShip.getPosition().x < BackGround.getGlobalBounds().width - gameWidth/2) {
                    spaceShip.move(ShipSpeed * deltaTime, 0.f);
                    spaceShip.setScale(-ShipScale,ShipScale);
                }
                // else{// Makes the ship go to the other side of the map when it reaches the end.
                //     spaceShip.setPosition(0.f, spaceShip.getPosition().y);
                //     GameView.move(-BackGround.getGlobalBounds().width + gameWidth+50, 0.f);
                // }
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

            return isleft;
            
}

//Could put inside the draw construstor of the SpaceShip.
void SpaceShip::GroundDraw(sf::RenderWindow& window){
    window.draw(BackGround);
}
