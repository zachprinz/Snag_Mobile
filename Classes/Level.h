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
#include "Game.h"
#include "cocos2d.h"
#include "tinyxml2.h"

USING_NS_CC;

class Level{
public:
    static Level* createWithValueMap(ValueMap);

    Level(bool isNew);
    
    void AddEntity(Entity*);
    void Add(Game* game);
    void Remove(Game* game);

    void SetName(std::string);
    void SetAuthor(std::string);
    void SetObjectID(std::string);
    void SetHighscoresID(std::string);
    std::string GetName();
    std::string GetPath();
    std::string GetAuthor();
    ValueMap getValueMap();
    std::vector<Entity*> ents;
    void Save();
    void makePublic();
    std::string GetStatus();
    std::string GetID();
    int GetFavorites();
    bool GetIsFavorited();
    Vec2 GetLaunchVelocity();
    Vec2 GetLaunchPosition();
    std::vector<Entity*> GetEntities();
    void Clear();
    std::string toString();
    void fromString(std::string);
    Entity* spawner;
    std::string GetLastSaveXML();
private:
    void CreateFromMapValues();
    bool hasMapObjects;
    void AddMapValue(Entity*);
    void SetMap(ValueMap);
    ValueMap map;
    std::string path;
    std::string name;
    std::string author;
    std::string objectID;
    std::string highscoresID;
    int entCount;
    bool saved;
};

#endif /* defined(__Snag__Level__) */
