#include "Bullet.h"

Bullet::Bullet(  sf::Vector2f position, int dir, float velocity)
    : velocity(velocity), direction(dir), active(false) {
    shape.setSize(sf::Vector2f(10, 2));
    shape.setPosition(position-sf::Vector2f(0,-37));
    shape.setFillColor(sf::Color::Green);
}

sf::FloatRect Bullet::getHitBox() {
    return shape.getGlobalBounds();
}

void Bullet::update() {
    if (active) {
        shape.move(velocity * direction, 0);
    }
}

void Bullet::draw(sf::RenderWindow& window) {
    if (active) {
        window.draw(shape);
    }
}

void Bullet::setActive(bool isActive) {
    active = isActive;
}

bool Bullet::isActive() const {
    return active;
}

sf::Vector2f Bullet::getPosition() const{
    return shape.getPosition();
}

void Bullet::removeInactiveBullets(std::vector<Bullet>& bullets) {
    bullets.erase(
        std::remove_if(bullets.begin(), bullets.end(), [](const Bullet& bullet) {
            return !bullet.isActive();
        }),
        bullets.end()
    );
}