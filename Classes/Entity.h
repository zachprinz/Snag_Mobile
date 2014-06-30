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

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

class Entity{
public:
    Entity(char* texture, int x = 500, int y = 300);
    Entity(char* texture, Vec2 pos);
    virtual void update(float dt);
    Sprite* GetSprite();
    void SetPosition(Point);
    Point GetPosition();
    void SetScale(Vec2);
    Vec2 GetScale();
    virtual Rect GetBounds();
    void SetBaseScale(Vec2 scale);
    void draw();
    static Vec2 userPosition;
    static float boardScale;
private:
    
protected:
    void UpdateSprite();
    virtual void CalculatePosition();
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
