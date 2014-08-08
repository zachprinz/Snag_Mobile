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
    static Level* createWithValueMap(ValueMap);

    Level();
    
    void AddEntity(MapObject*);
    void Add(Layer* game);

    void SetName(std::string);
    void SetAuthor(std::string);
    void SetObjectID(std::string);
    void SetHighscoresID(std::string);
    std::string GetName();
    std::string GetPath();
    std::string GetAuthor();
    ValueMap getValueMap();
    std::vector<MapObject*> GetMapObjects();
    std::vector<Entity*> ents;
private:
    void CreateFromMapValues();
    bool hasMapObjects;
    void AddMapObject(MapObject*);
    void AddMapValue(Entity*);
    void SetMap(ValueMap);
    ValueMap map;
    std::string path;
    std::string name;
    std::string author;
    std::string objectID;
    std::string highscoresID;
    std::vector<MapObject*> mapObjects;
    int entCount;
};

#endif /* defined(__Snag__Level__) */
