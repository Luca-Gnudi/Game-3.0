#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "doctest.h"
#include "Bullet.h" 

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
    // You may need to use a more advanced testing library for this, doctest isn't designed for graphical testing
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
