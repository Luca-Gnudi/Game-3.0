#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "lander.h"

////////////////////////////////////////////////////////////////////////////////////////////////////
//                                   Lander                                                       //
////////////////////////////////////////////////////////////////////////////////////////////////////

TEST_CASE("Lander starts to move towards its target") {
    sf::Vector2f initialPosition(100.0f, 100.0f); // Initial position
    sf::Vector2f targetPosition(200.0f, 200.0f); // Target position
    float deltaTime = 1.0f; // Time delta

    Lander lander(initialPosition); // Create a lander
    lander.updatePosition(targetPosition, deltaTime);


    // Check if the lander has moved towards the target position
    CHECK(lander.getPosition().x > initialPosition.x);
    CHECK(lander.getPosition().y > initialPosition.y);
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