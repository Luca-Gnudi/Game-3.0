#ifndef ENTITY_H
#define ENTITY_H

#include <SFML/Graphics.hpp>

class Entity {
public:
    Entity(const sf::Vector2f& startPosition);

    virtual sf::FloatRect getHitBox() const;
    virtual void updatePosition();
    virtual void draw(sf::RenderWindow& window);
    virtual void handleCollision(Entity& otherEntity);
    virtual void moveUp(float distance);

    sf::Vector2f getPosition() const;

private:
    sf::Vector2f position;
};

#endif