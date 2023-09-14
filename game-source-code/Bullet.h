#ifndef BULLET_H
#define BULLET_H

#include <SFML/Graphics.hpp>
#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(sf::Vector2f, int dir, float velocity);
    void update();
    void draw(sf::RenderWindow& window);
    void setActive(bool isActive);
    bool isActive() const;
    static void removeInactiveBullets(std::vector<Bullet>& bullets);
    sf::Vector2f getPosition() const;
    sf::RectangleShape shape;

private:
    
    float velocity;
    int direction;
    bool active;
};

#endif // BULLET_H
