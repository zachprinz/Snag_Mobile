//
//  Entity.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#ifndef __Snag__Entity__
#define __Snag__Entity__

#define MAX_WIDTH 2048
#define MAX_HEIGHT 1538
#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Game;
class Entity{
public:    
    static Entity* createWall(Vec2, Vec2);
    static Entity* createSpikeWall(Vec2, Vec2);
    static Entity* createGoal(Vec2, Vec2);
    static Entity* createHook(Vec2);
    static Entity* createSpawner(Vec2, Vec2);
    
    Entity(Vec2 pos, Vec2 size, Vec2 vel, int type);
    
    virtual void SetUpPhysicsSprite(std::string, float);
    virtual void CalculateScale(Vec2 userPosition, float boardScale);
    virtual void update(Vec2 userPosition, float boardScale);
    virtual void Add(Game* a);
    virtual void Remove(Game* a);
    void draw();
    
    Vec2 GetSize();
    Sprite* GetSprite();
    Point GetPosition();
    Vec2 GetLaunchVelocity();
    Rect GetBounds();
    Sprite* GetPhysicsSprite();
    int GetType();
protected:
    Vec2 size;
    Vec2 baseScale;
    Vec2 launchVelocity;
    Sprite* sprite;
    Point position;
    Vec2 imageSize;
    DrawNode* boundsDebug;
    PhysicsBody* body;
    Sprite* physicsSprite;
    int type;
};

#endif /* defined(__Snag__Entity__) */
