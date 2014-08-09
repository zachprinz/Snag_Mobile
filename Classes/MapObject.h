//
//  MapObject.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#ifndef __Snag__MapObject__
#define __Snag__MapObject__

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define ERASE 4
#define NO_TOOL 5
#define MOVE 6
#define GOAL 7
#define TILE_SIZE 40

#include <iostream>
#include "cocos2d.h"

using namespace cocos2d;

class MapObject{
public:
    static MapObject* CreateWithPosAndSize(Vec2,Vec2,int);
    MapObject(Vec2, int);
    MapObject(Vec2, Vec2, int);
    void UpdateEndCoord(cocos2d::Vec2);
    cocos2d::Sprite* GetSprite();
    void SetOriginTile(Vec2);
    static Vec2 origin;
    int GetType();
    Vec2 GetStart();
    Vec2 GetSize();
    void Add(Layer* game);
private:
    cocos2d::Vec2 startCoord;
    cocos2d::Vec2 endCoord;
    cocos2d::Sprite* sprite;
    int type;
};

#endif /* defined(__Snag__MapObject__) */
