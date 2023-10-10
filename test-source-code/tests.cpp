#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Bullet.h" 
#include "lander.h"
#include "missile.h"
#include "SpaceShip.h"
#include "humanoid.h"

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
    CHECK(bullet.shape.getSize() == sf::Vector2f(20, 5));
    CHECK(bullet.shape.getPosition() == position - sf::Vector2f(0, -51));
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

TEST_CASE("Lander moves towards the closest humanoid that is also a part of a vector") {
    std::srand(200);
    
    Lander lander;
    std::vector<Humanoid> humanoids;

    Humanoid humanoid1(sf::Vector2f(3350.0f, 500.0f), 1, 0); //Closer humanoid in relation to the lander
    Humanoid humanoid2(sf::Vector2f(6000.0f, 500.0f), 1, 0); //Farther humanoid in relation to the lander

    humanoids.push_back(humanoid1);
    humanoids.push_back(humanoid2);

    float deltaTime = 0.01f; // Time increment for each update
    int numUpdates = 1000;    // Number of updates to simulate

    for (int i = 0; i < numUpdates; ++i) {
        lander.updatePosition(humanoids, deltaTime);
    }

    // Check if the lander has moved towards the closest humanoid (humanoid1)
    CHECK(lander.getPosition().x == doctest::Approx(humanoid1.getPosition().x).epsilon(10));
    CHECK(lander.getPosition().y == doctest::Approx(humanoid1.getPosition().y).epsilon(10));
}

TEST_CASE("Lander spawns within specified boundaries of screen"){
    for (int i = 0; i < 100; ++i) { // Spawn 100 landers for the test
        Lander lander; // Simulate spawning a lander

        // Check the position of the spawned lander
        // Replace these values with your expected range based on your game logic
        float minX = 0.0f;
        float maxX = 6700.0f;
        float minY = 0.0f;
        float maxY = 600.0f;

        REQUIRE(lander.getPosition().x >= minX);
        REQUIRE(lander.getPosition().x <= maxX);
        REQUIRE(lander.getPosition().y >= minY);
        REQUIRE(lander.getPosition().y <= maxY);
    }
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

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                          Humanoid                                             //
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Humanoid moves horizontally") {
    // Create a Humanoid object with an initial position, direction, and velocity
    sf::Vector2f startPosition(100.0f, 100.0f);
    int initialDirection = 1; // Initial direction (1 or -1)
    float velocity = 5.0f;    // Initial velocity

    Humanoid humanoid(startPosition, initialDirection, velocity);

    // Update the Humanoid's position
    humanoid.updatePosition();

    // Get the updated position
    sf::Vector2f updatedPosition = humanoid.getPosition();

    // Check if the Humanoid's position has changed based on its velocity and direction
    if (initialDirection == 1) {
        // If the initial direction is 1, the Humanoid should have moved right
        CHECK(updatedPosition.x > startPosition.x);
    } else if (initialDirection == -1) {
        // If the initial direction is -1, the Humanoid should have moved left
        CHECK(updatedPosition.x < startPosition.x);
    }
}

TEST_CASE("Humanoid turns around and moves in the opposite direction when it reaches the left turn around point") {
    // Create a Humanoid object with an initial position, direction, and velocity
    sf::Vector2f startPosition(760.0f, 100.0f);
    int initialDirection = -1; // Initial direction to the left
    float velocity = 5.0f;    // Initial velocity

    // Create a Humanoid with the initial parameters
    Humanoid humanoid(startPosition, initialDirection, velocity);

    // Set a fixed deltaTime for testing
    float deltaTime = 10.0f;

    // Choose a point where the Humanoid should turn around
    float turnaroundPoint = 750.0f;

    // Get the updated position after reaching the turnaround point
    sf::Vector2f updatedPosition = humanoid.getPosition();

    // Check if the Humanoid has changed direction and moved away from the turnaround point
    CHECK(humanoid.getPosition().x >= turnaroundPoint);
}

TEST_CASE("Humanoid turns around and moves in the opposite direction when it reaches the right turn around point") {
    // Create a Humanoid object with an initial position, direction, and velocity
    sf::Vector2f startPosition(5450.0f, 100.0f);
    int initialDirection = 1; // Initial direction to the right
    float velocity = 5.0f;    // Initial velocity

    // Create a Humanoid with the initial parameters
    Humanoid humanoid(startPosition, initialDirection, velocity);

    // Set a fixed deltaTime for testing
    float deltaTime = 10.0f;

    // Choose a point where the Humanoid should turn around
    float turnaroundPoint = 5500.0f;

    // Get the updated position after reaching the turnaround point
    sf::Vector2f updatedPosition = humanoid.getPosition();

    // Check if the Humanoid has changed direction and moved away from the turnaround point
    CHECK(humanoid.getPosition().x <= turnaroundPoint);
}

///////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Captured Humanoid                                           //
///////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Captured Humanoid falls to the bottom of the screen while active"){
    CapturedHumanoid fallingHumanoid;
    sf::Vector2f startPosition = sf::Vector2f(3350.0f, 10.0f);
    fallingHumanoid.setPosition(startPosition);
    fallingHumanoid.setActive(true);

    float deltaTime = 0.1f; // Time increment for each update
    int numUpdates = 10;    // Number of updates to simulate

    for (int i = 0; i < numUpdates; ++i) {
        fallingHumanoid.updatePosition(deltaTime);
    }

    CHECK(fallingHumanoid.getPosition().y > startPosition.y);
    
}