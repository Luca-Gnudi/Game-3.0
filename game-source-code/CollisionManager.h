#ifndef CollisionManager_H
#define CollisionManager_H

#include "SpaceShip.h"
#include "lander.h"
#include "humanoid.h"
#include "CapturedHumanoid.h"
#include "Bullet.h"

class CollisionManager {
public:
    CollisionManager();

    bool checkMissileSpaceShipCollision(Missile& missile, SpaceShip& spaceShip);
    bool checkSpaceShipLanderCollision(SpaceShip& spaceShip, Lander& lander);
    bool checkBulletLanderCollision(Bullet& bullet, Lander& lander);
    bool checkHumanoidLanderCollision(Humanoid& humanoid, Lander& lander);
    bool checkBulletCapturedHumanoidCollision(Bullet& bullet, CapturedHumanoid& capturedHumanoid);
    bool checkSpaceShipCapturedHumanoidCollision(SpaceShip& spaceShip, CapturedHumanoid& capturedHumanoid);

private:

};

#endif