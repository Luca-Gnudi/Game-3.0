#include "Entity.h"

Entity::Entity(const sf::Vector2f& startPosition) : position(startPosition) {}

sf::FloatRect Entity::getHitBox() const {
    // Implement this in derived classes
    return sf::FloatRect();
}

void Entity::updatePosition() {
    // Implement this in derived classes
}

void Entity::draw(sf::RenderWindow& window) {
    // Implement this in derived classes
}

void Entity::handleCollision(Entity& otherEntity) {
    // Implement collision handling in derived classes
}

void Entity::moveUp(float distance) {
    position.y -= distance;
}

sf::Vector2f Entity::getPosition() const {
    return position;
}