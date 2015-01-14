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
#define USER 5


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
    static int count;
    
    Entity(Vec2 pos, Vec2 size, Vec2 vel, int type);
    
    virtual void SetUpPhysicsSprite(std::string, Vec2);
    virtual void SetUpParticles();
    virtual void update(Vec2 userPosition, float boardScale);
    virtual void Add(Game* a);
    virtual void Remove(Game* a);
    void draw();
    
    Vec2 GetSize();
    void SetSize(Vec2);
    Sprite* GetSprite();
    Point GetPosition();
    Vec2 GetLaunchVelocity();
    void SetLaunchVelocity(Vec2);
    int GetType();
    Vec2 GetOriginalSize();
    void SetPosition(Vec2);
    int ID;
    Sprite* line;
    void SetLine(float size, float angle);
    void SetLineOff();
    bool operator < (const Entity& str) const {return (position.x < str.position.x);}
    Point position;
    float GetSum();
    ParticleSystemQuad* emitter;
    LayerColor* color;
    bool hasParticleEffects;
protected:
    virtual void CalculateScale(Vec2 userPosition, float boardScale);
    Vec2 size;
    Vec2 baseScale;
    Vec2 launchVelocity;
    Sprite* sprite;
    Vec2 imageSize;
    DrawNode* boundsDebug;
    PhysicsBody* body;
    Vec2 originalSize;
    int type;
};

#endif /* defined(__Snag__Entity__) */
