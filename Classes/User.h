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
    virtual void Add(Game*);
    
    void Snag();
    void Release(bool add = true);
    void update(float dt);
    void collide(Vec2);
    void Reset();
    void Bounce(const PhysicsContactData* data);
    void SetBackupVelocity();
    void UpdateBounce();
    Vec2 GetPhysicalPosition();
    bool isHooked;
    Sprite* line;
    std::vector<Sprite*> pastSprites;
    void SetStretch(Sprite3D*, float angle, float magnitude);
    std::vector<PhysicsBody*> bodies;
    std::vector<Sprite*> sprites;
    std::vector<PhysicsJointDistance*> joints;
    PhysicsJointDistance* joint;
    std::vector<PhysicsJointDistance*> distanceJoints;
    float GetAngle(Vec2 a, Vec2 b);
    Entity* closest;
    Sprite* focusPointSprite;
    Vec2 GetClosestPosition();
    Sprite3D* sphere;
    SpotLight* light;
private:
    int tailCount;
    int spriteUpdateCount;
    void CalculateScale(Vec2 userPosition, float boardScale);
    Vec2 backupVelocity;
    Vec2 closestPosition;
    Entity* hook;
    void updatePhysics(float dt);
    float lineBaseScale;
};

#endif /* defined(__Snag__User__) */
