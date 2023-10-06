#include <SFML/Graphics.hpp>
#include <cmath>
#include "Bullet.h"
#include "lander.h"
#include "explosion.h"
#include "humanoid.h"
#include "capturedhumanoid.h"

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
    float scale = 2.5f;
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

    sf::Clock bulletCooldownTimer;
    const sf::Time bulletCooldownDuration = sf::seconds(0.5f); // Adjust the cooldown duration as needed
    bool canShootBullet = true;

    auto landerShot = 0;

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

    // Define a vector to store the landers
    std::vector<Lander> landers;
    std::vector<Explosion> explosions;

    std::vector<Humanoid> humanoids;
    std::vector<CapturedHumanoid> capturedHumanoids;
    std::vector<CapturedHumanoid> fallingHumanoids;
    // Define a timer for spawning landers
    sf::Clock landerSpawnTimer;
    float spawnInterval = 5.0f;

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

		    // Reset the position of space ship and clear alien objects
		    spaceShip.setPosition(spaceShipPosition);
            humanoids.clear();
            capturedHumanoids.clear();
            fallingHumanoids.clear();
            for (auto i=0; i<5; ++i){
                float xPosition = i * gameWidth / 5;
                float yPosition = gameHeight - 150;
                int direction = (i % 2 == 0) ? 1 : -1; // Alternating direction

                sf::Vector2f startPosition(xPosition, yPosition);
                Humanoid newHumanoid(startPosition, direction, 1.0);
                humanoids.push_back(newHumanoid);
            }
            landers.clear();
            explosions.clear();
            landerShot = 0;
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

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && canShootBullet){
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

             // Start the cooldown timer
             bulletCooldownTimer.restart();
             canShootBullet = false; // Prevent shooting until cooldown is over
            }

            // Check if the cooldown duration has passed
            if (!canShootBullet && bulletCooldownTimer.getElapsedTime() >= bulletCooldownDuration) {
                canShootBullet = true; // Allow shooting again
            }

            // Check if it's time to spawn a new lander
            if (landerSpawnTimer.getElapsedTime().asSeconds() >= spawnInterval) {
            // Create a new lander at a random position around the spaceship
            sf::Vector2f randomOffset;
            float distance = 300.0f; // Adjust the distance from the spaceship
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
              for (auto& lander : landers){
                for (auto& bullet : bullets) {
                    if (!bullet.isActive()) continue; // Skip inactive bullets

                    sf::FloatRect bulletHitBox = bullet.getHitBox();
                    if (lander.getHitBox().intersects(bulletHitBox)) {
                    // Bullet hit the lander
                    bullet.setActive(false); // Deactivate the bullet
                    lander.destroy();
                    landerShot++;
   
                    Explosion newExplosion(lander.getPosition(), 6, 0.005f);

                    // Restart the explosion animation
                    newExplosion.startAnimation();
                    explosions.push_back(newExplosion);

                    if (lander.isCarryingHumanoid){
                        for (auto& capturedHumanoid : capturedHumanoids){
                            capturedHumanoid.setActive(true);
                            fallingHumanoids.push_back(capturedHumanoid);
                            lander.carryingHumanoid(false);
                        }
                    }

                    // Remove destroyed landers from the vector
                    landers.erase(std::remove_if(landers.begin(), landers.end(),[](const Lander& lander) { return !lander.isActive(); }), landers.end());

                    // Optionally, you can handle other actions when the lander is destroyed

                    break; // Exit the loop early, as we only need to handle one collision
                    }
                  
                  if (landerShot == 10){
                    isPlaying = false;
                    pauseMessage.setString("YOU WON!\nNow go get some fresh air!");
                    playInstructions.setString("Press Esc to exit the game.");
                    break;
                  }
                }
              }
            for (Lander& lander : landers) {
                for (Humanoid& humanoid : humanoids) {
                    sf::Vector2f direction = humanoid.getPosition() - lander.getPosition();
                    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

                    // Adjust the distance threshold as needed
                    if (distance < 10) {
                    lander.captureHumanoid(humanoid);
                    // Remove the captured humanoid from the vector
                    humanoids.erase(std::remove_if(humanoids.begin(), humanoids.end(),
                    [&humanoid](const Humanoid& h) { return &h == &humanoid; }),
                    humanoids.end());

                    sf::Vector2f capturedHumanoidOffset;
                    capturedHumanoidOffset = sf::Vector2f(0.0f, 50);
                    CapturedHumanoid newCapturedHumanoid;
                    newCapturedHumanoid.setPosition(lander.getPosition() + capturedHumanoidOffset);
                    newCapturedHumanoid.setActive(false);
                    capturedHumanoids.push_back(newCapturedHumanoid);
                    }
                }
                if (humanoids.empty()){
                    isPlaying = false;
                    pauseMessage.setString("Game Over!\nAll astronauts captured!");
                    break;
                }
            }
            for (Lander& lander : landers) {
                if (lander.isCarryingHumanoid) {
                    lander.moveWithHumanoid(deltaTime);

                    // Check if the lander is offscreen at the top and release the humanoid
                    if (lander.getPosition().y < 10 ) {
                    lander.isCarryingHumanoid = false;
                    // You can perform additional actions here, e.g., award points
                    }
                }
            }
              
        }
       
         //rendering
        window.clear();
        window.draw(Background);
        if(isPlaying){

            float deltaTime = clock.restart().asSeconds();
            Bullet::removeInactiveBullets(bullets);
            window.draw(spaceShip);

            // Call updateMissile to handle missile shooting
           for (auto& lander : landers){
            lander.missileShoot(deltaTime, gameWidth, gameHeight, spaceShip.getPosition());
            // Update and draw the enemy
                lander.updatePosition(humanoids, deltaTime);
                lander.draw(window);
                lander.missileDraw(window);

                // Draw captured humanoids
                if (lander.isCarryingHumanoid) {
                for (auto& capturedHumanoid : capturedHumanoids){
                sf::Vector2f capturedHumanoidOffset = sf::Vector2f(0.0f, 50);
                capturedHumanoid.setPosition(lander.getPosition() + capturedHumanoidOffset);
                capturedHumanoid.updatePosition(deltaTime);
                capturedHumanoid.draw(window);
                }
                }
                
           }

            for (auto& bullet : bullets) {
                bullet.update();
                bullet.draw(window);
            }

            for (auto& explosion : explosions) {
                explosion.update(deltaTime);
            }

            for (auto& explosion : explosions) {
                if (!explosion.isFinished()) {
                    explosion.draw(window);
                }
            }

            for (auto& humanoid : humanoids){
                humanoid.updatePosition();
            }

            for (auto& humanoid : humanoids) {
                humanoid.draw(window);
            }

            for (auto& capturedHumanoid : capturedHumanoids){
                capturedHumanoid.updatePosition(deltaTime);
                capturedHumanoid.draw(window);
            }
            for (auto& fallingHumanoid : fallingHumanoids){
                fallingHumanoid.updatePosition(deltaTime);
                fallingHumanoid.draw(window);
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