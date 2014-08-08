//
//  Level.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#include "Level.h"
#include "tinyxml2.h"
#include "Goal.h"
#include "Wall.h"
#include "SpikeWall.h"
#include "MapObject.h"
#include "Hook.h"
#include "Spawner.h"

Level* Level::createWithValueMap(ValueMap map){
    Level* level = new Level();
    level->SetName(map["name"].asString());
    level->SetAuthor(map["author"].asString());
    level->SetMap(map);
    return level;
};

Level::Level(){
    entCount = 0;
}
void Level::AddEntity(MapObject* e){
    mapObjects.push_back(e);
    Entity* ent;
    Vec2 pos = e->GetStart();
    Vec2 size;
    Vec2 vel;
    if(e->GetType() == WALL || e->GetType() == SPIKE_WALL || e->GetType() == GOAL)
        size = e->GetSize();
    if(e->GetType() == SPAWNER)
        vel.set(100,600);
    switch(e->GetType()){
        case WALL:
            ent = new Wall(pos,size);
            break;
        case GOAL:
            ent = new Goal(pos,size);
            break;
        case SPIKE_WALL:
            ent = new SpikeWall(pos, size);
            break;
        case SPAWNER:
            ent = new Spawner(pos, vel);
            break;
        case HOOK:
            ent = new Hook(pos);
            break;
    }
    AddMapValue(ent);
    ents.push_back(ent);
}
void Level::AddMapValue(Entity* ent){
    std::string pre = "Entity_" + std::to_string(entCount) + "_";
    map[pre + "typeString"] = ent->typeString;
    map[pre + "type"] = ent->GetType();
    map[pre + "x"] = ent->GetPosition().x;
    map[pre + "y"] = ent->GetPosition().y;
    map[pre + "width"] = 0;
    map[pre + "height"] = 0;
    map[pre + "velocityX"] = 0;
    map[pre + "velocityY"] = 0;
    if(ent->GetType() == WALL || ent->GetType() == SPIKE_WALL || ent->GetType() == GOAL){
        map[pre + "width"] = ent->GetSize().x;
        map[pre + "height"] = ent->GetSize().y;
    }
    if(ent->GetType() == SPAWNER){
        map[pre + "velocityX"] = ((Spawner*)ent)->GetStartVelocity().x;
        map[pre + "velocityY"] = ((Spawner*)ent)->GetStartVelocity().y;
    }
    entCount++;
    map["count"] = entCount;
}
void Level::CreateFromMapValues(){
    for(int x = 0; x < entCount; x++){
        std::string pre = "Entity_" + std::to_string(x) + "_";
        Vec2 pos(map[pre + "x"].asInt(),map[pre + "y"].asInt());
        Vec2 size(map[pre + "width"].asInt(), map[pre + "height"].asInt());
        Vec2 vel(map[pre + "velocityX"].asInt(), map[pre + "velocityY"].asInt());
        int type = map[pre + "type"].asInt();
        AddEntity(MapObject::CreateWithPosAndSize(pos,size,type));
    }
    hasMapObjects = true;
}
std::vector<MapObject*> Level::GetMapObjects(){
    if(hasMapObjects)
        return mapObjects;
    CreateFromMapValues();
    return mapObjects;
}
void Level::Add(Layer* game){
    for(int x = 0; x < ents.size(); x++){
        ents[x]->Add(game);
    }
}
ValueMap Level::getValueMap(){
    return map;
};
std::string Level::GetPath(){
    return path;
}
std::string Level::GetName(){
    return name;
}
std::string Level::GetAuthor(){
    return author;
    //Todo, Include author info
}
void Level::SetName(std::string n){
    name = n;
}
void Level::SetAuthor(std::string a){
    author = a;
}
void Level::SetObjectID(std::string o){
    objectID = o;
}
void Level::SetHighscoresID(std::string h){
    highscoresID = h;
}
void Level::SetMap(ValueMap map){
    this->map = map;
    entCount = map["count"].asInt();
    hasMapObjects = false;
    if(entCount == 0)
        hasMapObjects = true;
}