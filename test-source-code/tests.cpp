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