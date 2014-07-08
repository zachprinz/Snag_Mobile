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

class Entity{
public:
    Entity(char* texture, int x = 500, int y = 300, int type = 0);
    virtual void update(float dt);
    Sprite* GetSprite();
    Point GetPosition();
    void SetScale(Vec2);
    Vec2 GetScale();
    virtual Rect GetBounds();
    void SetBaseScale(Vec2 scale);
    void draw();
    static Vec2 userPosition;
    static float boardScale;
    void UpdateSprite();
    virtual void SetUpPhysicsSprite(char*);
    Sprite* physicsSprite;
    PhysicsBody* body;
    int type;
private:
    
protected:
    virtual void CalculateScale();
    static Vec2 baseScale;
    Vec2 spriteBaseScale;
    Sprite* sprite;
    Point position;
    Vec2 scale;
    Vec2 imageSize;
    DrawNode* boundsDebug;
};

#endif /* defined(__Snag__Entity__) */
