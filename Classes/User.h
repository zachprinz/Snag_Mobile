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

USING_NS_CC;

class User : public Entity{
public:
    static int type;
    User();
    virtual void SetUpPhysicsSprite(std::string, Vec2);
    virtual void Add(Layer*);
    
    void Snag();
    void Release();
    void update(float dt);
    void collide(Vec2);
    void Reset();
    void Bounce(const PhysicsContactData* data);
    void SetBackupVelocity();
    void UpdateBounce();
    Vec2 GetPhysicalPosition();
private:
    void CalculateScale(Vec2 userPosition, float boardScale);
    Vec2 backupVelocity;
    Vec2 closestPosition;
    PhysicsJointDistance* joint;
    Sprite* line;
    Entity* hook;
    Entity* closest;
    bool isHooked;
    void updatePhysics(float dt);
    float lineBaseScale;
    float GetAngle(Vec2 a, Vec2 b);
};

#endif /* defined(__Snag__User__) */
