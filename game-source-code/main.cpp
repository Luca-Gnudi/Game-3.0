#include <SFML/Graphics.hpp>
#include <cmath>
#include "Bullet.h"
#include "lander.h"
#include "explosion.h"


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

    //Create a hitbox for the space ship
    sf::FloatRect spaceShipHitBox;

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

    
    bool landerDestroyed = false;
    // Define a vector to store the landers
    std::vector<Lander> landers;
    // Define a timer for spawning landers
    sf::Clock landerSpawnTimer;
    float spawnInterval = 5.0f;

    auto isleft = true;//bool for the direction that the lazer shoots.
    
    std::vector<Bullet> bullets;
    auto bulletSpeed = 40.f;

    Explosion explosion(sf::Vector2f(-1000, -1000),6, 0.005f);

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

            // Check if it's time to spawn a new lander
            if (landerSpawnTimer.getElapsedTime().asSeconds() >= spawnInterval) {
            // Create a new lander at a random position around the spaceship
            sf::Vector2f randomOffset;
            float distance = 200.0f; // Adjust the distance from the spaceship
            float angle = static_cast<float>(std::rand() % 360); // Random angle in degrees
            randomOffset.x = std::cos(angle * 3.14159265f / 180) * distance;
            randomOffset.y = std::sin(angle * 3.14159265f / 180) * distance;

            // Calculate the lander's position relative to the spaceship
            sf::Vector2f landerPosition = spaceShip.getPosition() + randomOffset;

            // Create the lander and add it to the vector
            Lander newLander(landerPosition);
            landers.push_back(newLander);

            // Restart the spawn timer with a new random interval
            spawnInterval = 2.0f + static_cast<float>(std::rand()) / (RAND_MAX / 6.0f);
            landerSpawnTimer.restart();

            }


            if (isleft){
            spaceShipHitBox.left = spaceShip.getPosition().x + 8*scale;
            spaceShipHitBox.top = spaceShip.getPosition().y + 10*scale;
            spaceShipHitBox.width = 14*scale;
            spaceShipHitBox.height = 10*scale;
            }
            else{
            spaceShipHitBox.left = spaceShip.getPosition().x - 20*scale;
            spaceShipHitBox.top = spaceShip.getPosition().y + 10*scale;
            spaceShipHitBox.width = 14*scale;
            spaceShipHitBox.height = 10*scale;
            }

            // Check collision with missiles
            for (auto& lander : landers){
            for (size_t i = 0; i < lander.missiles.size(); ++i) {
               if (spaceShipHitBox.intersects(lander.missiles[i].getHitBox())) {
                  isPlaying = false;
                  pauseMessage.setString("Game Over!\nPress Enter to restart or\nescape to exit");
                  break; // Exit the loop early, as we already detected a collision
                }
            }
            }

            for (auto& lander : landers){
            sf::FloatRect landerHitBox = lander.getHitBox();
            if (isCollision(spaceShipHitBox,landerHitBox)){
                isPlaying = false;
                pauseMessage.setString("Game Over!\nPress Enter to restart or\nescape to exit");
            }
            }
            // Check collision between bullets and the lander
            if (!landerDestroyed) {
              for (auto& lander : landers){
                for (auto& bullet : bullets) {
                    if (!bullet.isActive()) continue; // Skip inactive bullets

                    sf::FloatRect bulletHitBox = bullet.getHitBox();
                    if (lander.getHitBox().intersects(bulletHitBox)) {
                    // Bullet hit the lander
                    bullet.setActive(false); // Deactivate the bullet
                    landerDestroyed = true; // Mark the lander as destroyed
   
                    // Set the explosion position to the lander's position
                    explosion.setPosition(lander.getPosition());

                    // Restart the explosion animation
                    explosion.startAnimation();

                    // Remove destroyed landers from the vector
                    landers.erase(std::remove_if(landers.begin(), landers.end(),[](const Lander& lander) { return !lander.isActive(); }), landers.end());

                    // Clear any existing missiles
                    lander.missiles.clear();

                    // Optionally, you can handle other actions when the lander is destroyed

                    break; // Exit the loop early, as we only need to handle one collision
                    }
                }
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
           for (auto& lander : landers){
            lander.missileShoot(deltaTime, gameWidth, gameHeight, spaceShip.getPosition());
            // Update and draw the enemy
            if (isleft){
                lander.updatePosition(spaceShip.getPosition() + sf::Vector2f(16*scale,0), deltaTime);
            }
            else{
                lander.updatePosition(spaceShip.getPosition() - sf::Vector2f(16*scale,0), deltaTime);
            }
            if (!landerDestroyed){
                
                    lander.updatePosition(spaceShip.getPosition() + sf::Vector2f(16*scale,0), deltaTime);
                    lander.draw(window);
                    lander.missileDraw(window);
                
            }
            else{
                // Draw the explosion animation
                explosion.update(deltaTime);
                explosion.draw(window);

                // Check if the explosion animation has finished
                if (explosion.isFinished()) {
                landerDestroyed = false;
                }
            }
           }
        }
        else{
            // Draw the pause message
	        window.draw(pauseMessage);
            window.draw(playInstructions);
        }
        
        window.display();


}
 
}