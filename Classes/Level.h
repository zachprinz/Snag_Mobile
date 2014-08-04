//
//  Level.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#ifndef __Snag__Level__
#define __Snag__Level__

#include <iostream>
#include "Wall.h"
#include "SpikeWall.h"
#include "Hook.h"
#include "Spawner.h"
#include "Goal.h"
#include "MapObject.h"
#include "cocos2d.h"

USING_NS_CC;

class Level{
public:
    Level(std::string path, std::string name);
    static Level* createWithXML(std::string);
    static Level* createWithMapObjects(std::vector<MapObject*>, std::string name);
    //void AddEntity(Entity*);
    void SetMapObjects(std::vector<MapObject*>);
    bool createdMapObjects;
    std::string GetName();
    std::string GetPath();
    std::string GetAuthor();
    std::vector<Entity*> ents;
    void AddEntity(Entity*);
    void SetName(std::string);
    void Add(Layer* game);
private:
    void AddMapObject(MapObject*);
    std::string path;
    std::string name;
    std::vector<MapObject*> mapObjects;
};

#endif /* defined(__Snag__Level__) */
