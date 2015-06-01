#ifndef __Snag__Level__
#define __Snag__Level__

#include <iostream>
#include "cocos2d.h"
#include "tinyxml2.h"
#include "Entity.h"

USING_NS_CC;

class Level{
public:
    Level();
    Level(ValueMap);

    void setName(std::string);
    void setAuthor(std::string);
    void setPublic(bool);
    
    std::string getName();
    std::string getAuthor();
    std::string getID();

    bool getPublic();
    int getNumFavorites();
    bool getIsFavorited();

    std::vector<Entity*> getEntities();
    void                 setEntities(std::vector<Entity*>);
    
    void clear();
    void save();

private:
    ValueMap map;
    
    bool uploaded;

    std::vector<Entity*> mapToEntities();
    void entitiesToMap(std::vector<Entity*>);

    void addMapValue(Entity*);
    void setMap(ValueMap);
};

#endif /* defined(__Snag__Level__) */
