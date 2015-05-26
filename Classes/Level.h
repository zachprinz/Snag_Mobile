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
    
    void addEntity(Entity*);
    void add(Game* game);
    void remove(Game* game);
    Entity* getEntity(int ID);
    void setName(std::string);
    void setAuthor(std::string);
    void setObjectID(std::string);
    void setHighscoresID(std::string);
    std::string getName();
    std::string getPath();
    std::string getAuthor();
    ValueMap getValueMap();
    void save();
    void makePublic();
    std::string getStatus();
    std::string getID();
    int getFavorites();
    bool getIsFavorited();
    Vec2 getLaunchVelocity();
    Vec2 getLaunchPosition();
    std::vector<Entity*> getEntities();
    void clear();
    std::string toString();
    void fromString(std::string);
    Entity* spawner;
    std::string getLastSaveXML();
private:
    std::vector<Entity*> ents;
    void createFromMapValues();
    bool hasMapObjects;
    void addMapValue(Entity*);
    void setMap(ValueMap);
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
