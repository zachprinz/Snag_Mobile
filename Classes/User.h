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
    void SetUpPhysicsSprite(char*);
    static int type;
    void Bounce(const PhysicsContactData* data);
    void SetBackupVelocity(Vec2);
    Point GetPhysicsPosition();
    void UpdateBounce();
    void CalculateScale();

private:
    Vec2 backupVelocity;
    PhysicsJointDistance* joint;
    Hook* hook;
    bool isHooked;
    void updatePhysics(float dt);
    Vec2 closestPosition;
    Sprite* line;
    float lineBaseScale;
    Hook* closest;
    float GetAngle(Vec2 a, Vec2 b);
};

#endif /* defined(__Snag__User__) */
