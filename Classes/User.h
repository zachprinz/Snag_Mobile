//
//  User.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#ifndef __Snag__User__
#define __Snag__User__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"
#include "Hook.h"

USING_NS_CC;

class User : public Entity{
public:
    User();
    void Snag();
    void Release();
    void update(float dt);
    void collide(Vec2);
    void Reset();
private:
    void CalculateScale();
    PhysicsJointDistance* joint;
    Hook* hook;
    bool isHooked;
    void updatePhysics(float dt);
    Vec2 velocity;
    Vec2 acceleration;
    float angularVelocity;
    float hookArmAngle;
    float hookArmDistance;
    Vec2 closestPosition;
    void SetPositionToArmAngle();
    void FindAngularVelocity();
    Sprite* line;
    float lineBaseScale;
    Hook* closest;
};

#endif /* defined(__Snag__User__) */
