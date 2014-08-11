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
#include "MapObject.h"
#include "Game.h"
#include "cocos2d.h"

USING_NS_CC;

class Level{
public:
    static Level* createWithValueMap(ValueMap);

    Level();
    
    void AddEntity(MapObject*);
    void Add(Game* game);
    void AddToMap(Layer* game);
    void Remove(Game* game);

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
    void Save();
    void makePublic();
    std::string GetStatus();
    std::string GetID();
    int GetFavorites();
    bool GetIsFavorited();
    Vec2 GetLaunchVelocity();
    Vec2 GetLaunchPosition();
private:
    void CreateFromMapValues();
    bool hasMapObjects;
    void AddMapObject(MapObject*);
    void AddMapValue(Entity*, Vec2,Vec2);
    void SetMap(ValueMap);
    ValueMap map;
    std::string path;
    std::string name;
    std::string author;
    std::string objectID;
    std::string highscoresID;
    std::vector<MapObject*> mapObjects;
    Vec2 launchVelocity;
    Vec2 launchPosition;
    int entCount;
};

#endif /* defined(__Snag__Level__) */
