//
//  Preview.h
//  Snag
//
//  Created by Zachary Prinzbach on 8/13/14.
//
//

#ifndef __Snag__Preview__
#define __Snag__Preview__

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

class Preview{
public:
    Preview(Rect viewport, Layer* layer, float scale);
    void AddEntity(Entity* ent);
    void RemoveEntity(Entity* ent);
    void RemoveEntity(int entID);
    Entity* PopEntity(Entity* ent);
    Entity* PopEntity(int entID);
    void SetZoom(float);
    void Drag(Vec2);
    Entity* CreateEntity(Vec2 startTouch, Vec2 endTouch, int type);
    Entity* GetTarget(Vec2 touch);
    Vec2 ScreenToMap(Vec2);
    Vec2 MapToScreen(Vec2);
    void Update();
    void Reset();
    void onTouchesBegan(std::vector<Touch*>);
    void onTouchesMoved(std::vector<Touch*>);
    void onTouchesEnded(std::vector<Touch*>);
    float GetScale();
    bool editor;
    void Clear();
private:
    ClippingNode* clipNode;
    DrawNode* stencil;
    float originalMultitouchDistance;
    float currentMultitouchDistance;
    void UpdateSprite(int);
    std::map<int,Entity*> entities;
    std::map<int,Sprite*> sprites;
    Rect screenViewport;
    Vec2 screenViewOrigin;
    Vec2 screenViewSize;
    Vec2 mapViewOrigin;
    float mapViewScale;
    float mapViewScaleOriginal;
    Layer* layer;
    float originalMapViewScale;
};

#endif /* defined(__Snag__Preview__) */
