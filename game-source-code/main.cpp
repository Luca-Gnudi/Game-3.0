#include <SFML/Graphics.hpp>
#include <cmath>
#include "Bullet.h"
#include "lander.h"
#include "explosion.h"
#include "humanoid.h"
#include <SFML/Audio.hpp>
#include <iostream>
#include "Bullet.h"
#include "lander.h"
#include "SpaceShip.h"

// Collision detection logic
bool isCollision(const sf::FloatRect& rect1, const sf::FloatRect& rect2) {
    return rect1.intersects(rect2);
}

int main(){
    const int gameWidth = 1600; //The width of the game screen.
    const int gameHeight = 900; //The height of the game screen.
    
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 50), "Defender Game", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);//Helps reduce toll on processing.
    sf::Texture BackgroundTexture;
    if(!BackgroundTexture.loadFromFile("resources/Space-mountain.png")){
        return EXIT_FAILURE;
    }
   
    sf::Sprite Background;
    Background.setTexture(BackgroundTexture);
    Background.setPosition(0,0);
    //Background.setScale(0.67f,0.67f);

    //Settting up a game view to allow for scrolling.
    sf::View GameView;
    GameView.reset(sf::FloatRect(0,0,gameWidth,gameHeight));
    //GameView.setViewport(sf::FloatRect(0,0,0.6f,0.6f));
    
    //Setting up a MiniMap View.
    sf::View MiniMapView;
    MiniMapView.reset(sf::FloatRect(0.f,0.f,Background.getGlobalBounds().width,Background.getGlobalBounds().height));
    MiniMapView.setViewport(sf::FloatRect(0.2f,0.f,0.66f,0.15f));

    //Setting up a higlight square for the minimap. possibly try use the game view.
    sf::RectangleShape CurrentView(sf::Vector2f(gameWidth,gameHeight));
    CurrentView.setOrigin(gameWidth/2,gameHeight/2);
    CurrentView.setFillColor(sf::Color::Transparent);
    CurrentView.setOutlineThickness(-10);
    CurrentView.setOutlineColor(sf::Color::Red);
    
    sf::Texture spaceShipTexture;
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png")) {
        return EXIT_FAILURE;
    }

    // Create a sprite for the space ship and set its texture
    float scale = 2.5f;

    //Create the space ship.
    const int ShipSize = 175;
    sf::Vector2f spaceShipPosition((Background.getGlobalBounds().width/2)-ShipSize, gameHeight/2);
    const float shipSpeed = 1000.f;

    //Create a hitbox for the space ship
    sf::FloatRect spaceShipHitBox;

    sf::Clock bulletCooldownTimer;
    const sf::Time bulletCooldownDuration = sf::seconds(0.5f); // Adjust the cooldown duration as needed
    bool canShootBullet = true;

    auto landerShot = 0;

    SpaceShip spaceShip(scale, shipSpeed, spaceShipPosition, Background); // Create the spaceShip

    // Load the text font
    sf::Font font;
    if(!font.loadFromFile("resources/sansation.ttf"))
	return EXIT_FAILURE;

    // Initialise Game instructions
    sf::Text playInstructions;
    playInstructions.setFont(font);
    playInstructions.setCharacterSize(20);
    playInstructions.setPosition(gameWidth/2-400, gameHeight/2+100);
    playInstructions.setFillColor(sf::Color::White);
    playInstructions.setString("Use WASD to move and space to shoot");

    // Initialize the pause message
    sf::Text pauseMessage;
    pauseMessage.setFont(font);
    pauseMessage.setCharacterSize(50);
    pauseMessage.setPosition(gameWidth/2-400, gameHeight/2-50);
    pauseMessage.setFillColor(sf::Color::White);
    pauseMessage.setString("Welcome to Defender!\nPress Enter to start the game");

    // Define the space ship's properties.
    sf::Clock AITimer;
    const sf::Time AITime = sf::seconds(0.1f);

    