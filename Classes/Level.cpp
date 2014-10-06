//
//  Level.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#include "Level.h"
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
void Level::AddEntity(Entity* e){
    ents.push_back(e);
    AddMapValue(e);
}
void Level::AddMapValue(Entity* ent){
    Vec2 pos = ent->GetPosition();
    Vec2 size = ent->GetSize();
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
	log("CreateFromMapValues()");
    ents.clear();
    log("\tFinished Clearing");
    ValueVector entities = map["entities"].asValueVector();
    log("\tCreated ValueVector");
    int count = std::atoi(map["entcount"].asString().c_str());
    log("\tEntering Loop");
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
        if(type == SPAWNER){
            launchPosition = pos;
            launchVelocity = velocity;
        }
        //printf("\nType: %i\nPos: (%f, %f)\nSize: (%f, %f)\nVelocity: (%f, %f)\n", type, pos.x, pos.y, size.x, size.y, velocity.x, velocity.y);
        log("\tAdding Entity");
        AddEntity(new Entity(pos,size,Vec2(0,0),type));
    }
    hasMapObjects = true;
}
std::vector<Entity*> Level::GetEntities(){
	log("Getting Entities");
    if(hasMapObjects){
    	log("\thasMapObjects");
        return ents;
    }
    CreateFromMapValues();
    return ents;
}
void Level::Save(){
    map["name"] = name;
    Value parameters = Value(map);
    sendMessageWithParams("saveLevel", parameters);
}
void Level::Add(Game* game){
    GetEntities();
    for(int x = 0; x < ents.size(); x++){
        ents[x]->Add(game);
    }
}
void Level::Remove(Game* game){
    GetEntities();
    for(int x = 0; x < ents.size(); x++){
        ents[x]->Remove(game);
    }
}
void Level::makePublic(){
    map["status"] = "Public";
    Value parameters = Value(map);
    sendMessageWithParams("saveLevelInfo", parameters);
}
void Level::Clear(){
    ents.clear();
    for(int x = 0; x < entCount; x++){
        std::string pre = "entity" + std::to_string(ents.size());
        map.erase(pre + "type");
        map.erase(pre + "x");
        map.erase(pre + "y");
        map.erase(pre + "width");
        map.erase(pre + "height");
        map.erase(pre + "velocityX");
        map.erase(pre + "velocityY");
    }
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
