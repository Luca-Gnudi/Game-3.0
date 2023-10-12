#include "Bullet.h"
#include "lander.h"
#include "explosion.h"
#include "humanoid.h"
#include "capturedhumanoid.h"
#include "SpaceShip.h"
#include "CollisionManager.h"

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

    sf::Clock clock;
    auto isPlaying = false;
    auto SetInstructionPosition = false;

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

    CollisionManager collisionManager;

    while (window.isOpen()){
        
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

		    // Reset the position of space ship and clear all other objects
            humanoids.clear();
            capturedHumanoids.clear();
            fallingHumanoids.clear();
            for (auto i=0; i<5; ++i){
                auto landWidth = gameWidth * 5;
                float xPosition = i * landWidth / 5;
                float yPosition = gameHeight - 150;
                int direction = (i % 2 == 0) ? 1 : -1; // Alternating direction

                sf::Vector2f startPosition(xPosition, yPosition);
                Humanoid newHumanoid(startPosition, direction, 0.5f);
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
            isleft = spaceShip.SpaceShipControl(deltaTime, bulletSpeed, gameWidth, gameHeight, bullets, GameView, window);
            sf::FloatRect spaceShipHitBox = spaceShip.getHitBox();
            // Check if it's time to spawn a new lander
            if (landerSpawnTimer.getElapsedTime().asSeconds() >= spawnInterval) {

            //sf::Vector2f randomOffset;
            float distance = 600.0f; // Adjust the distance from the spaceship

            float y_min = gameHeight*0.15;
            float y_max = gameHeight-300.f;

            // Create the lander and add it to the vector
            Lander newLander;
            landers.push_back(newLander);

            // Restart the spawn timer with a new random interval
            spawnInterval = 2.0f + static_cast<float>(std::rand()) / (RAND_MAX / 6.0f);
            landerSpawnTimer.restart();

            }

            // Check collision with missiles
            for (auto& lander : landers){
            for (size_t i = 0; i < lander.missiles.size(); ++i) {
               if (collisionManager.checkMissileSpaceShipCollision(lander.missiles[i],spaceShip)) {
                  isPlaying = false;
                  SetInstructionPosition = true;
                  pauseMessage.setString("Game Over!\nPress Enter to restart or\nescape to exit");
                  break; // Exit the loop early, as we already detected a collision
                }
            }
            }

            for (auto& lander : landers){
            if (collisionManager.checkSpaceShipLanderCollision(spaceShip, lander)){
                isPlaying = false;
                SetInstructionPosition = true;
                pauseMessage.setString("Game Over!\nPress Enter to restart or\nescape to exit");
            }
            }
            // Check collision between bullets and the lander
              for (auto& lander : landers){
                for (auto& bullet : bullets) {
                    if (!bullet.isActive()) continue; // Skip inactive bullets

                    if (collisionManager.checkBulletLanderCollision(bullet, lander)) {
                    // Bullet hit the lander
                    bullet.setActive(false); // Deactivate the bullet
                    lander.destroy();
                    landerShot++;
   
                    Explosion newExplosion(lander.getPosition(), 6, 0.005f);
                    newExplosion.startAnimation();
                    explosions.push_back(newExplosion);

                    if (lander.isCarryingHumanoid){
                        for (auto& capturedHumanoid : capturedHumanoids){
                            capturedHumanoid.setActive(true);
                            capturedHumanoid.setPosition(lander.getPosition());
                            fallingHumanoids.push_back(capturedHumanoid);
                            lander.carryingHumanoid(false);

                            capturedHumanoids.erase(std::remove_if(capturedHumanoids.begin(), capturedHumanoids.end(),
                            [&capturedHumanoid](const CapturedHumanoid& h) { return &h == &capturedHumanoid; }),
                            capturedHumanoids.end());
                        }
                    }

                    // Remove destroyed landers from the vector
                    landers.erase(std::remove_if(landers.begin(), landers.end(),[](const Lander& lander) { return !lander.isActive(); }), landers.end());

                    break; // Exit the loop early, as we only need to handle one collision
                    }
                    
                    for (auto& capturedHumanoid : capturedHumanoids){
                        if (collisionManager.checkBulletCapturedHumanoidCollision(bullet, capturedHumanoid)){

                            lander.carryingHumanoid(false);

                            Explosion newExplosion(capturedHumanoid.getPosition(), 6, 0.005f);
                            newExplosion.startAnimation();
                            explosions.push_back(newExplosion);

                            capturedHumanoids.erase(std::remove_if(capturedHumanoids.begin(), capturedHumanoids.end(),
                            [&capturedHumanoid](const CapturedHumanoid& h) { return &h == &capturedHumanoid; }),
                            capturedHumanoids.end());

                        }
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
                    if (collisionManager.checkHumanoidLanderCollision(humanoid, lander)) {
                    lander.captureHumanoid(humanoid);
                    // Remove the captured humanoid from the vector
                    humanoids.erase(std::remove_if(humanoids.begin(), humanoids.end(),
                    [&humanoid](const Humanoid& h) { return &h == &humanoid; }),
                    humanoids.end());

                    sf::Vector2f capturedHumanoidOffset;
                    capturedHumanoidOffset = sf::Vector2f(0.0f, 50);
                    CapturedHumanoid newCapturedHumanoid;
                    newCapturedHumanoid.setActive(false);
                    capturedHumanoids.push_back(newCapturedHumanoid);
                    }
                }

            }
            for (Lander& lander : landers) {
                if (lander.isCarryingHumanoid) {

                    // Check if the lander is offscreen at the top and release the humanoid
                    if (lander.getPosition().y < 10 ) {
                    lander.isCarryingHumanoid = false;
                    for (auto& capturedHumanoid : capturedHumanoids){
                        capturedHumanoids.erase(std::remove_if(capturedHumanoids.begin(), capturedHumanoids.end(),
                        [&capturedHumanoid](const CapturedHumanoid& h) { return &h == &capturedHumanoid; }),
                        capturedHumanoids.end());
                    }
                    }
                }
            }

            for (auto& fallingHumanoid : fallingHumanoids){
                sf::Vector2f direction = fallingHumanoid.getPosition() - spaceShip.getPosition();
                float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
                if (collisionManager.checkSpaceShipCapturedHumanoidCollision(spaceShip, fallingHumanoid) || (isleft && distance < 60) || (!isleft && distance < 90)){
                    fallingHumanoid.setActive(false);
                    if (isleft){
                        sf::Vector2f fallingHumanoidOffset;
                        fallingHumanoidOffset = sf::Vector2f(0.0f, 40.0f);
                        fallingHumanoid.setPosition(spaceShip.getPosition() + fallingHumanoidOffset);
                    }
                    else{
                       sf::Vector2f fallingHumanoidOffset;
                       fallingHumanoidOffset = sf::Vector2f(-50.0f, 40.0f);
                       fallingHumanoid.setPosition(spaceShip.getPosition() + fallingHumanoidOffset);
                    }
                }

                if (fallingHumanoid.getPosition().y > 700 && fallingHumanoid.isActive()){
                    Explosion newExplosion(fallingHumanoid.getPosition(), 6, 0.005f);
                    newExplosion.startAnimation();
                    explosions.push_back(newExplosion);

                    fallingHumanoids.erase(std::remove_if(fallingHumanoids.begin(), fallingHumanoids.end(),
                    [&fallingHumanoid](const CapturedHumanoid& h) { return &h == &fallingHumanoid; }),
                    fallingHumanoids.end());
                } else if (fallingHumanoid.getPosition().y > 600 && !fallingHumanoid.isActive()){
                    fallingHumanoids.erase(std::remove_if(fallingHumanoids.begin(), fallingHumanoids.end(),
                    [&fallingHumanoid](const CapturedHumanoid& h) { return &h == &fallingHumanoid; }),
                    fallingHumanoids.end());
                    
                    float xPosition = spaceShip.getPosition().x;
                    float yPosition = gameHeight - 150;

                    sf::Vector2f startPosition(xPosition, yPosition);
                    Humanoid newHumanoid(startPosition, 1.0, 0.5f);
                    humanoids.push_back(newHumanoid);
                }

                for (auto& bullet : bullets){
                    if (collisionManager.checkBulletCapturedHumanoidCollision(bullet, fallingHumanoid)){
                        Explosion newExplosion(fallingHumanoid.getPosition(), 6, 0.005f);
                        newExplosion.startAnimation();
                        explosions.push_back(newExplosion);

                        fallingHumanoids.erase(std::remove_if(fallingHumanoids.begin(), fallingHumanoids.end(),
                        [&fallingHumanoid](const CapturedHumanoid& h) { return &h == &fallingHumanoid; }),
                        fallingHumanoids.end());
                    }
                }
            }

            for (auto& bullet : bullets){
                bullet.update();
            }

            if (humanoids.empty() && capturedHumanoids.empty() && fallingHumanoids.empty()){
                isPlaying = false;
                pauseMessage.setString("Game Over!\nAll astronauts captured!");
            }

        }

         //rendering
        window.clear();
        if(isPlaying){
            
            GameView.setCenter(spaceShip.getPosition().x,gameHeight/2);


            //Draw objects in the main view.
            window.setView(GameView);
            window.draw(Background);
            spaceShip.draw(window);

            float deltaTime = clock.restart().asSeconds();
            Bullet::removeInactiveBullets(bullets);
            spaceShip.draw(window);

            for (auto& bullet : bullets) {
                bullet.draw(window);
            }
        
              for (auto& humanoid : humanoids){
                humanoid.updatePosition();
            }

            for (auto& humanoid : humanoids) {
                humanoid.draw(window);
            }

            for (auto& bullet : bullets) {
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
        
           // Call updateMissile to handle missile shooting
           for (auto& lander : landers){
            lander.missileShoot(deltaTime, gameWidth, gameHeight, spaceShip.getPosition());
            // Update and draw the enemy
                lander.updatePosition(humanoids, deltaTime);
                lander.draw(window);
                lander.missileDraw(window);

                // Draw captured humanoids
                if (lander.isCarryingHumanoid) {
                CapturedHumanoid capturedHumanoid;
                sf::Vector2f capturedHumanoidOffset;
                capturedHumanoidOffset = sf::Vector2f(0.0f, 50);
                capturedHumanoid.setPosition(lander.getPosition() + capturedHumanoidOffset);
                
                capturedHumanoid.draw(window);
                }
                
           }

           for (auto& capturedHumanoid : capturedHumanoids){
                capturedHumanoid.updatePosition(deltaTime);
                capturedHumanoid.draw(window);
            }

           for (auto& fallingHumanoid : fallingHumanoids){
                fallingHumanoid.updatePosition(deltaTime);
                fallingHumanoid.draw(window);
            }     

            //Drawing the minimap view.
            window.setView(MiniMapView);

            window.draw(Background);
            CurrentView.setPosition(spaceShip.getPosition().x, gameHeight/2);//Set the position of the highlighted view.
            window.draw(CurrentView);
            spaceShip.draw(window);
            

            for (auto& bullet : bullets) {
                bullet.draw(window);
            }
        
               for (auto& humanoid : humanoids){
                humanoid.updatePosition();
            }

            for (auto& humanoid : humanoids) {
                humanoid.draw(window);
            }

            for (auto& bullet : bullets) {
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
        
           // Call updateMissile to handle missile shooting
           for (auto& lander : landers){
            lander.missileShoot(deltaTime, gameWidth * 5, gameHeight, spaceShip.getPosition());
            // Update and draw the enemy
                lander.updatePosition(humanoids, deltaTime);
                lander.draw(window);
                lander.missileDraw(window);

                // Draw captured humanoids
                if (lander.isCarryingHumanoid) {
                CapturedHumanoid capturedHumanoid;
                sf::Vector2f capturedHumanoidOffset;
                capturedHumanoidOffset = sf::Vector2f(0.0f, 50);
                capturedHumanoid.setPosition(lander.getPosition() + capturedHumanoidOffset);
                
                capturedHumanoid.draw(window);
                }
                
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
            GameView.setCenter(spaceShip.getPosition().x,gameHeight/2);
            window.setView(GameView);
            window.draw(Background);
            spaceShip.draw(window);
            // Draw the pause message
	        //if (SetInstructionPosition){
                playInstructions.setPosition(spaceShip.getPosition().x - gameWidth/3,gameHeight/4+180);
                pauseMessage.setPosition(spaceShip.getPosition().x - gameWidth/3, gameHeight/4);
            //}
        
            window.draw(playInstructions);
            window.draw(pauseMessage);
        
        }
        
        window.display();


}
 
}