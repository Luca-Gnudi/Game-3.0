#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Bullet.h" 
#include "lander.h"
#include "missile.h"

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