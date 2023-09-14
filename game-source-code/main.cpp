#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "lander.h"

int main(){
    const int gameWidth = 1600; //The width of the game screen.
    const int gameHeight = 900; //The height of the game screen.
    
    sf::RenderWindow window(sf::VideoMode(gameWidth, gameHeight, 50), "Defender Game", sf::Style::Titlebar | sf::Style::Close);
    window.setVerticalSyncEnabled(true);
    window.setFramerateLimit(60);//Helps reduce toll on processing.
    sf::Texture BackgroundTexture;
    if(!BackgroundTexture.loadFromFile("resources/Space-Background-Image-7.jpg")){
        return EXIT_FAILURE;
    }
   
    sf::Sprite Background;
    Background.setTexture(BackgroundTexture);
    Background.setPosition(0,0);
    
    sf::Texture spaceShipTexture;
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png")) {
        return EXIT_FAILURE;
    }

    // Create a sprite for the space ship and set its texture
    sf::Sprite spaceShip;
    float scale = 5.5f;
    spaceShip.setTexture(spaceShipTexture);
    spaceShip.setScale(scale, scale); // Adjust the scale as needed

    //Create the space ship.
    const int ShipSize = 175;
    sf::Vector2f spaceShipPosition(gameWidth-ShipSize, gameHeight/2);
    spaceShip.setPosition(spaceShipPosition);
    spaceShip.setOrigin(sf::Vector2f(scale,scale));
    const float shipSpeed = 1000.f;

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

    sf::Clock clock;
    auto isPlaying = false;

    sf::Vector2f centerPosition(gameWidth / 2.0f, gameHeight / 2.0f);
    Lander lander(centerPosition); // Specify the initial position

    auto isleft = true;//bool for the direction that the lazer shoots.
    
    std::vector<Bullet> bullets;
    auto bulletSpeed = 40.f;
    while (window.isOpen()){
        
     //bool isLazers = false;//bool to determine if lazers should be drawn.
        sf::Event event;
        while (window.pollEvent(event))
        {
            // Window closed or escape key pressed: exit
	        if((event.type == sf::Event::Closed) ||
	            ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))) {
		    window.close();
		    break;
	        }

	    // Enter key pressed: play
	    if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Enter)) {
		if(!isPlaying) {
		    // (re)start the game
		    isPlaying = true;
		    clock.restart();

		    // Reset the position of the paddles and ball
		    spaceShip.setPosition(spaceShipPosition);
		}
	    }
	}
       if (isPlaying) {
            float deltaTime = clock.restart().asSeconds();

            // Moving the space ship vertically and horizontally
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && (spaceShip.getPosition().y > 0)) {
                spaceShip.move(0.f, -shipSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && (spaceShip.getPosition().y + spaceShip.getLocalBounds().height < gameHeight-75)) {
                spaceShip.move(0.f, shipSpeed * deltaTime);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) && (spaceShip.getPosition().x > 25)) {
                spaceShip.move(-shipSpeed * deltaTime, 0.f);
                spaceShip.setScale(scale,scale);
                isleft = true;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D) && (spaceShip.getPosition().x + spaceShip.getLocalBounds().width < gameWidth)) {
                spaceShip.move(shipSpeed * deltaTime, 0.f);
                spaceShip.setScale(-scale,scale);
                isleft = false;
            }
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
            }
}

            for (auto& bullet : bullets) {
                bullet.update();
            }


         //rendering
        window.clear();
        window.draw(Background);
        if(isPlaying){

            float deltaTime = clock.restart().asSeconds();
            Bullet::removeInactiveBullets(bullets);
            window.draw(spaceShip);

            for (auto& bullet : bullets) {
                bullet.draw(window);
            }
        
           // Call updateMissile to handle missile shooting
            lander.missileShoot(deltaTime, gameWidth, gameHeight, spaceShip.getPosition());
            // Update and draw the enemy
            lander.updatePosition(spaceShip.getPosition(), deltaTime);
            lander.draw(window);
            lander.missileDraw(window);
        }
        else{
            // Draw the pause message
	        window.draw(pauseMessage);
            window.draw(playInstructions);
        }
        
        window.display();


}
 
}


  


