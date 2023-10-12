#include "CollisionManager.h"

CollisionManager::CollisionManager() {
    // Initialize any collision-related data structures if needed
}

bool CollisionManager::checkMissileSpaceShipCollision(Missile& missile, SpaceShip& spaceShip) {
    sf::FloatRect missileHitBox = missile.getHitBox();
    sf::FloatRect spaceShipHitBox = spaceShip.getHitBox();

    return missileHitBox.intersects(spaceShipHitBox);
}

bool CollisionManager::checkSpaceShipLanderCollision(SpaceShip& spaceShip, Lander& lander) {
    sf::FloatRect spaceShipHitBox = spaceShip.getHitBox();
    sf::FloatRect landerHitBox = lander.getHitBox();

    return spaceShipHitBox.intersects(landerHitBox);
}

bool CollisionManager::checkBulletLanderCollision(Bullet& bullet, Lander& lander) {
    sf::FloatRect bulletHitBox = bullet.getHitBox();
    sf::FloatRect landerHitBox = lander.getHitBox();

    return bulletHitBox.intersects(landerHitBox);
}

bool CollisionManager::checkHumanoidLanderCollision(Humanoid& humanoid, Lander& lander) {
    sf::FloatRect humanoidHitBox = humanoid.getHitBox();
    sf::FloatRect landerHitBox = lander.getHitBox();

    return humanoidHitBox.intersects(landerHitBox);
}

bool CollisionManager::checkBulletCapturedHumanoidCollision(Bullet& bullet, CapturedHumanoid& capturedHumanoid) {
    sf::FloatRect bulletHitBox = bullet.getHitBox();
    sf::FloatRect capturedHumanoidHitBox = capturedHumanoid.getHitBox();

    return bulletHitBox.intersects(capturedHumanoidHitBox);
}

bool CollisionManager::checkSpaceShipCapturedHumanoidCollision(SpaceShip& spaceShip, CapturedHumanoid& capturedHumanoid) {
    sf::FloatRect spaceShipBounds = spaceShip.getHitBox();
    sf::FloatRect capturedHumanoidBounds = capturedHumanoid.getHitBox();

    return spaceShipBounds.intersects(capturedHumanoidBounds);
}