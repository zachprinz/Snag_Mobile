#include "Level.h"
#include "NDKHelper/NDKHelper.h"
#include "User.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

#define SPAWNER 3


Level* Level::createWithValueMap(ValueMap map){
    Level* level = new Level(false);
    level->SetName(map["name"].asString());
    level->SetAuthor(map["author"].asString());
    if(map.find("data") != map.end())
        level->fromString(map["data"].asString());
    level->SetMap(map);
    //Broken Here
    return level;
};

Level::Level(bool isNew){
    entCount = 0;
    if(isNew){
        this->fromString("<Map></Map>");
        saved = false;
    }
}
void Level::AddEntity(Entity* e){
    ents.push_back(e);
    if(e->GetType() == SPAWNER)
        spawner = e;
}
std::vector<Entity*> Level::GetEntities(){
    if(hasMapObjects){
        return ents;
    }
    return ents;
}
void Level::Save(){
    map["name"] = name;
    map["data"] = toString();
    Value parameters = Value(map);
    if(saved)
        sendMessageWithParams("saveLevel", parameters);
    else
        sendMessageWithParams("newLevel", parameters);
    saved = true;
}
std::string Level::GetLastSaveXML(){
    if(map.find("data") != map.end()){
        return map["data"].asString();
    }
    return "";
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
    Save();
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
    Vec2 launchVelocity = spawner->GetLaunchVelocity();
    return launchVelocity;
    
}
Vec2 Level::GetLaunchPosition(){
    return spawner->GetPosition();
}
std::string Level::GetName(){
    if(!name.empty())
        return name;
    return "none";
}
std::string Level::GetAuthor(){
    if(author.empty() && map.find("author") != map.end())
        author = map["author"].asString();
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
bool compById(Entity* a, Entity* b)
{
    return a->GetSum() < b->GetSum();
}
std::string Level::toString(){
    std::sort(ents.begin(), ents.begin() + ents.size(), compById);
    for(int x = 0; x < ents.size(); x++){
        printf("\nposition.x = %f\tGetPosition().x = %f", ents[x]->position.x, ents[x]->GetPosition().x);
    }
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* map = doc.NewElement("Map");
    map->SetAttribute("name", this->GetName().c_str());
    map->SetAttribute("author", this->GetAuthor().c_str());
    std::vector<Entity*> tempEnts;
    for(int x = 0; x < ents.size(); x++){
        if(ents[x] != NULL){
            tinyxml2::XMLElement* entity = doc.NewElement("entity");
            //entity->SetAttribute("ID", ents[x]->ID);
            entity->SetAttribute("type", ents[x]->GetType());
            entity->SetAttribute("x", ents[x]->GetPosition().x);
            entity->SetAttribute("y", ents[x]->GetPosition().y);
            entity->SetAttribute("width", ents[x]->GetSize().x);
            entity->SetAttribute("height", ents[x]->GetSize().y);
            entity->SetAttribute("xVelocity", ents[x]->GetLaunchVelocity().x);
            entity->SetAttribute("yVelocity", ents[x]->GetLaunchVelocity().y);
            map->InsertFirstChild(entity);
            tempEnts.push_back(ents[x]);
        }
    }
    ents = tempEnts;
    doc.InsertFirstChild(map);
    std::string returnMap;
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    return printer.CStr();
};
void Level::fromString(std::string xmlString){
    Clear();
    tinyxml2::XMLDocument doc;
    doc.Parse(xmlString.c_str());
    if(doc.ErrorID())
        printf("Error Parsing Map from String");
    tinyxml2::XMLElement* root = doc.RootElement();
    if(root->FirstChildElement() != NULL){
        tinyxml2::XMLElement* iterEnt = root->FirstChild()->ToElement();
        bool first = true;
        do{
            if(!first){
                iterEnt = iterEnt->NextSiblingElement();
            } else {
                first = false;
            }
            Vec2 tempPos(iterEnt->IntAttribute("x"), iterEnt->IntAttribute("y"));
            Vec2 tempSize(iterEnt->IntAttribute("width"), iterEnt->IntAttribute("height"));
            Vec2 tempVelocity(iterEnt->IntAttribute("xVelocity"), iterEnt->IntAttribute("yVelocity"));
            int tempType = iterEnt->IntAttribute("type");
            Entity* tempEnt = new Entity(tempPos,tempSize,tempVelocity,tempType);
            if(tempType == SPAWNER){
                spawner = tempEnt;
            }
            tempSize = tempEnt->GetSize();
            AddEntity(tempEnt);
        } while(iterEnt->NextSiblingElement());
    }
    hasMapObjects = true;
    saved = true;
};