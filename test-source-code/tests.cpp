#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "SpaceShip.h"
#include "Bullet.h" 
#include "lander.h"
#include "missile.h"
#include "explosion.h"

#include <vector>


///////////////////////////////////////////////////////////////////////////////////////////////////
//                                     ScreenScrolling                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("GameView moves right"){
    const int gameWidth = 1600; // The width of the game screen.
    const int gameHeight = 900; // The height of the game screen.

    // Setting up a game view to allow for scrolling.
    sf::View GameView;
    GameView.reset(sf::FloatRect(0, 0, gameWidth, gameHeight));

    const float shipSpeed = 1000.f;

    float deltaTime = 100.f;

    GameView.move(shipSpeed * deltaTime, 0.f);

    CHECK(GameView.getCenter().x > gameWidth/2);

}

TEST_CASE("GameView moves left"){
    const int gameWidth = 1600; // The width of the game screen.
    const int gameHeight = 900; // The height of the game screen.

    // Setting up a game view to allow for scrolling.
    sf::View GameView;
    GameView.reset(sf::FloatRect(0, 0, gameWidth, gameHeight));

    const float shipSpeed = 1000.f;

    float deltaTime = 100.f;

    GameView.move(-shipSpeed * deltaTime, 0.f);

    CHECK(GameView.getCenter().x < gameWidth/2);

}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                      Current View                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////
TEST_CASE("Current View moves"){
    const int gameWidth = 1600; // The width of the game screen.
    const int gameHeight = 900; // The height of the game screen.

    sf::Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("resources/Space-mountain.png"))
    {
        std::cout << "Can't find file" << std::endl;
    }

    sf::Sprite Background;
    Background.setTexture(BackgroundTexture);
    Background.setPosition(0, 0);
    Background.setScale(1.0f, 1.0f);
    sf::Texture spaceShipTexture;
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png"))
    {
        std::cout << "Can't find file" << std::endl;
    }

    // Create a sprite for the space ship and set its texture
    float scale = 2.5f;
    const int ShipSize = 175;
    sf::Vector2f spaceShipPosition((Background.getGlobalBounds().width / 2) - ShipSize, gameHeight / 2);
    const float shipSpeed = 1000.f;

    sf::Sprite spaceShip;
    spaceShip.setTexture(spaceShipTexture);
    spaceShip.setScale(scale,scale);
    spaceShip.setPosition(spaceShipPosition);
    spaceShip.setOrigin(scale,scale);

    float deltaTime = 100.f;

    // Setting up a higlight square for the minimap. possibly try use the game view.
    sf::RectangleShape CurrentView(sf::Vector2f(gameWidth, gameHeight));
    CurrentView.setOrigin(gameWidth / 2, gameHeight / 2);
    CurrentView.setFillColor(sf::Color::Transparent);
    CurrentView.setOutlineThickness(-10);
    CurrentView.setOutlineColor(sf::Color::Red);

    SUBCASE("Current View moves left"){
        spaceShip.move(-shipSpeed*deltaTime, 0.f);
        CurrentView.setPosition(spaceShip.getPosition().x, gameHeight / 2);
        CHECK(CurrentView.getPosition().x == spaceShip.getPosition().x);
    }

    SUBCASE("Current View moves right"){
        spaceShip.move(-shipSpeed*deltaTime, 0.f);
        CurrentView.setPosition(spaceShip.getPosition().x, gameHeight / 2);
        CHECK(CurrentView.getPosition().x == spaceShip.getPosition().x);
    }
}



///////////////////////////////////////////////////////////////////////////////////////////////////
//                                        SpaceShip                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("SpaceShip Movement"){
    const int gameWidth = 1600; // The width of the game screen.
    const int gameHeight = 900; // The height of the game screen.

    sf::Texture BackgroundTexture;
    if (!BackgroundTexture.loadFromFile("resources/Space-mountain.png"))
    {
        std::cout << "Can't find file" << std::endl;
    }

    sf::Sprite Background;
    Background.setTexture(BackgroundTexture);
    Background.setPosition(0, 0);
    Background.setScale(1.0f, 1.0f);

    // Settting up a game view to allow for scrolling.
    sf::View GameView;
    GameView.reset(sf::FloatRect(0, 0, gameWidth, gameHeight));
    
    sf::Texture spaceShipTexture;
    if (!spaceShipTexture.loadFromFile("resources/assets/space_ship.png"))
    {
        std::cout << "Can't find file" << std::endl;
    }

    // Create a sprite for the space ship and set its texture
    float scale = 2.5f;
    const int ShipSize = 175;
    sf::Vector2f spaceShipPosition((Background.getGlobalBounds().width / 2) - ShipSize, gameHeight / 2);
    const float shipSpeed = 1000.f;

    sf::Sprite spaceShip;
    spaceShip.setTexture(spaceShipTexture);
    spaceShip.setScale(scale,scale);
    spaceShip.setPosition(spaceShipPosition);
    spaceShip.setOrigin(scale,scale);

    float deltaTime = 100.f;

    SUBCASE("SpaceShip moves left"){
        spaceShip.move(-shipSpeed * deltaTime, 0.f);
        spaceShip.move(-shipSpeed * deltaTime, 0.f);
        spaceShip.move(-shipSpeed * deltaTime, 0.f);
        CHECK(spaceShip.getPosition().x < spaceShipPosition.x);
    }

    spaceShipPosition = spaceShip.getPosition();

    SUBCASE("SpaceShip moves right"){
        spaceShip.move(shipSpeed * deltaTime, 0.f);
        spaceShip.move(shipSpeed * deltaTime, 0.f);
        spaceShip.move(shipSpeed * deltaTime, 0.f);
        CHECK(spaceShip.getPosition().x > spaceShipPosition.x);
    }

    spaceShipPosition = spaceShip.getPosition();

    SUBCASE("SpaceShip moves up"){
        spaceShip.move(0.f, -shipSpeed * deltaTime);
        spaceShip.move(0.f, -shipSpeed * deltaTime);
        spaceShip.move(0.f, -shipSpeed * deltaTime);
        CHECK(spaceShip.getPosition().y < spaceShipPosition.y);
    }

    spaceShipPosition = spaceShip.getPosition();

    SUBCASE("SpaceShip moves down"){
        spaceShip.move(0.f, shipSpeed * deltaTime);
        spaceShip.move(0.f, shipSpeed * deltaTime);
        spaceShip.move(0.f, shipSpeed * deltaTime);
        CHECK(spaceShip.getPosition().y > spaceShipPosition.y);
    }


}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Bullet                                               //
///////////////////////////////////////////////////////////////////////////////////////////////////

//Checks that bullet is created properly.
TEST_CASE("Bullet initialization") {
    sf::Vector2f position(100, 100);
    int dir = 1;
    float velocity = 5.0f;

    Bullet bullet(position, dir, velocity);

    CHECK(bullet.isActive() == false);
    CHECK(bullet.shape.getSize() == sf::Vector2f(10, 2));
    CHECK(bullet.shape.getPosition() == position - sf::Vector2f(0, -37));
    CHECK(bullet.shape.getFillColor() == sf::Color::Green);
}

//Test that the bullet moves correctly.
TEST_CASE("Bullet update") {
    sf::Vector2f position(100, 100);
    int dir = 1;
    float velocity = 5.0f;

    Bullet bullet(position, dir, velocity);

    bullet.setActive(true);
    bullet.update();

    CHECK(bullet.shape.getPosition().x == doctest::Approx(105.0).epsilon(0.4));
    CHECK(bullet.shape.getPosition().y == doctest::Approx(100.0).epsilon(0.4));

}

TEST_CASE("Bullet draw") {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Test Window");
    window.clear();

    sf::Vector2f position(100, 100);
    int dir = 1;
    float velocity = 5.0f;

    Bullet bullet(position, dir, velocity);

    bullet.setActive(true);
    bullet.draw(window);

    // Manually check if bullet is drawn correctly in the window
}

TEST_CASE("Bullet setActive") {
    sf::Vector2f position(100, 100);
    int dir = 1;
    float velocity = 5.0f;

    Bullet bullet(position, dir, velocity);

    bullet.setActive(true);
    CHECK(bullet.isActive() == true);

    bullet.setActive(false);
    CHECK(bullet.isActive() == false);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Lander                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Lander moves towards a target over time") {
    // Create a SpaceShip and set its initial position
    sf::Vector2f spaceshipPosition(100.0f, 100.0f);
    sf::Sprite backgroundSprite; // Create a valid background sprite here

    SpaceShip spaceShip(1.0f, 100.0f, spaceshipPosition, backgroundSprite);

    // Create a Lander and set its initial position and speed
    float distance = 200.0f; // Adjust the distance as needed
    float bound_x = 0.0f;   // Adjust the bounds as needed
    float bound_y = 1000.0f;
    Lander lander(distance, spaceShip, bound_x, bound_y);

    // Set the lander's initial position (you may adjust this as needed)
    sf::Vector2f initialLanderPosition(500.0f, 500.0f);
    lander.updatePosition(initialLanderPosition, 1.0f); // Simulate one second of movement

    // Get the updated position of the lander
    sf::Vector2f updatedLanderPosition = lander.getPosition();

    // Check if the lander has moved closer to the spaceship
    CHECK(updatedLanderPosition.x != doctest::Approx(initialLanderPosition.x));
    CHECK(updatedLanderPosition.y != doctest::Approx(initialLanderPosition.y));
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Missile                                              //
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Missile moves towards target position") {
    // Create a target position (space ship's position)
    sf::Vector2f targetPosition(100.0f, 200.0f);

    // Create a missile at some initial position
    sf::Vector2f initialPosition(50.0f, 50.0f);
    Missile missile(initialPosition, targetPosition);

    // Set a fixed deltaTime for testing
    float deltaTime = 10.0f;

    // Update the missile's position based on deltaTime
    missile.updatePosition(deltaTime);

    // Check if the missile has moved towards the target position
    sf::Vector2f newPosition = missile.getPosition();

    // Assert that the missile's new position is closer to the target position
    CHECK(newPosition.x > initialPosition.x);
    CHECK(newPosition.y > initialPosition.y);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Explosion                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////

// Helper function to simulate the passage of time
void simulateTime(Explosion& explosion, float deltaTime, int totalFrames, float frameDuration) {
    for (int frame = 0; frame < totalFrames; ++frame) {
        explosion.update(deltaTime);
    }
}

TEST_CASE("Explosion's full animation occurs") {
    // Create an Explosion object with the desired parameters
    sf::Vector2f position(100.0f, 100.0f);
    int totalFrames = 6;        // Adjust the total number of frames
    float frameDuration = 0.1f; // Adjust the frame duration

    Explosion explosion(position, totalFrames, frameDuration);

    // Start the animation
    explosion.startAnimation();

    // Simulate time to progress through the animation
    float deltaTime = frameDuration * totalFrames;
    simulateTime(explosion, deltaTime, totalFrames, frameDuration);

    // Check if the animation has finished
    CHECK(explosion.isFinished());
}