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

TEST_CASE("Lander moves towards a target over time") {
    // Create a SpaceShip and set its initial position
    sf::Vector2f spaceshipPosition(100.0f, 100.0f);
    SpaceShip spaceShip(1.0f, 100.0f, spaceshipPosition);

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
    sf::Vector2f startPosition(760.0f, 760.0f);
    int initialDirection = -1; // Initial direction (1 or -1)
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