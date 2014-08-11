//
//  Level.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#include "Level.h"
#include "MapObject.h"
#include "NDKHelper/NDKHelper.h"
#include "User.h"

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
    Vec2 size(0,0);
    Vec2 vel(0,0);
    if(e->GetType() == WALL || e->GetType() == SPIKE_WALL || e->GetType() == GOAL)
        size = e->GetSize();
    if(e->GetType() == SPAWNER){
        vel.set(100,600);
        launchVelocity = Vec2(100,600);
        launchPosition = pos;
    }
    switch(e->GetType()){
        case WALL:
            ent = Entity::createWall(pos,size);
            break;
        case SPIKE_WALL:
            ent = Entity::createSpikeWall(pos,size);
            break;
        case GOAL:
            ent = Entity::createGoal(pos,size);
        default:
            ent = new Entity(pos, size, vel, e->GetType());
    }
    
    ents.push_back(ent);
    AddMapValue(ent, pos, size);
}
void Level::AddMapValue(Entity* ent, Vec2 pos, Vec2 size){
    std::string pre = "entity" + std::to_string(ents.size());
    map[pre + "type"] = std::to_string(ent->GetType());
    map[pre + "x"] = std::to_string(pos.x);
    map[pre + "y"] = std::to_string(pos.y);
    map[pre + "width"] = "0";
    map[pre + "height"] = "0";
    map[pre + "velocityX"] = "0";
    map[pre + "velocityY"] = "0";
    if(ent->GetType() == WALL || ent->GetType() == SPIKE_WALL || ent->GetType() == GOAL){
        map[pre + "width"] = std::to_string(ent->GetSize().x);
        map[pre + "height"] = std::to_string(ent->GetSize().y);
    }
    if(ent->GetType() == SPAWNER){
        map[pre + "velocityX"] = std::to_string(ent->GetLaunchVelocity().x);
        map[pre + "velocityY"] = std::to_string(ent->GetLaunchVelocity().y);
    }
    map["entcount"] = std::to_string(ents.size());
}
void Level::CreateFromMapValues(){
    ents.clear();
    mapObjects.clear();
    ValueVector entities = map["entities"].asValueVector();
    int count = std::atoi(map["entcount"].asString().c_str());
    for (int i = 0; i < count; i++) {
        int z = i*7;
        Vec2 pos;
        Vec2 size;
        Vec2 velocity;
        int type;
        type = std::atoi(entities.at(z).asString().c_str());
        pos.x = std::atof(entities.at(z+1).asString().c_str());
        pos.y = std::atof(entities.at(z+2).asString().c_str());
        size.x = std::atof(entities.at(z+3).asString().c_str());
        size.y = std::atof(entities.at(z+4).asString().c_str());
        velocity.x = std::atof(entities.at(z+5).asString().c_str());
        velocity.y = std::atof(entities.at(z+6).asString().c_str());
        printf("\nType: %i\nPos: (%f, %f)\nSize: (%f, %f)\nVelocity: (%f, %f)\n", type, pos.x, pos.y, size.x, size.y, velocity.x, velocity.y);
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
void Level::Save(){
    map["name"] = name;
    Value parameters = Value(map);
    sendMessageWithParams("saveLevel", parameters);
}
void Level::Add(Game* game){
    GetMapObjects();
    for(int x = 0; x < ents.size(); x++){
        ents[x]->Add(game);
    }
}
void Level::Remove(Game* game){
    GetMapObjects();
    for(int x = 0; x < ents.size(); x++){
        ents[x]->Remove(game);
    }
}
void Level::AddToMap(Layer* game){
    for(int x = 0; x < ents.size(); x++){
        mapObjects[x]->Add(game);
    }
}
void Level::makePublic(){
    map["status"] = "Public";
    Value parameters = Value(map);
    sendMessageWithParams("saveLevelInfo", parameters);
}
ValueMap Level::getValueMap(){
    return map;
};
std::string Level::GetPath(){
    return path;
}
Vec2 Level::GetLaunchVelocity(){
    return launchVelocity;
}
Vec2 Level::GetLaunchPosition(){
    return launchPosition;
}
std::string Level::GetName(){
    return name;
}
std::string Level::GetAuthor(){
    return author;
    //Todo, Include author info
}
int Level::GetFavorites(){
    return std::atoi(map["favorites"].asString().c_str());
}
std::string Level::GetStatus(){
    return map["status"].asString();
}
std::string Level::GetID(){
    return map["id"].asString();
}
bool Level::GetIsFavorited(){
    if(map["favorited"].asString().compare("true") == 0)
        return true;
    return false;
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
    entCount = std::atoi(map["entcount"].asString().c_str());
    hasMapObjects = false;
    if(entCount == 0)
        hasMapObjects = true;
}