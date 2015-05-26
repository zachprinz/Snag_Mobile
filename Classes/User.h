#ifndef __Snag__User__
#define __Snag__User__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class User : public Entity{
    
public:
    User();
    virtual void setUpPhysicsSprite(bool collision);
    
    virtual void add();
    virtual void remove();
    
    void snag();
    void release(bool add = true);
    
    void update();
    void collide(Vec2);
    void reset(Vec2, Vec2);
    void Bounce(const PhysicsContactData* data);
    bool isHooked();
    Sprite* line;
    
    std::vector<Sprite*> pastSprites;
    void setStretch(Sprite*, float angle, float magnitude);

    Vec2 getClosestPosition();
    void setLine(float size, float angle);
    void setLineOff();
    Vec2 getVelocity();
    
private:
    bool hooked;
    int tailCount;
    int spriteUpdateCount;
    void updatePosition();
    void updateSpriteStretch();
    Vec2 backupVelocity;
    Vec2 closestPosition;
    Entity* hook;
    void updatePhysics(float dt);
    float lineBaseScale;
    Entity* closest;

    Sprite* focusPointSprite;
    Sprite* targetFocusPointSprite;
    
    std::vector<Sprite*> sprites;
    PhysicsJointDistance* joint;
};

#endif /* defined(__Snag__User__) */
