#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "Bullet.h" 

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
