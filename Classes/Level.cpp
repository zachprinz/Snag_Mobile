#include "Level.h"
#include "NDKHelper/NDKHelper.h"
#include "User.h"
#include <string.h>
#include <iostream>
#include <vector>
#include <algorithm>

Level::Level(){
    map["data"] = "<Map></Map>";
    uploaded = false;
}

Level::Level(ValueMap map){
    setMap(map);
    uploaded = true;
}

std::vector<Entity*> Level::getEntities(){
    return mapToEntities();
}

void Level::setEntities(std::vector<Entity*> entities){
    clear();
    entitiesToMap(entities);
}

void Level::save(){
    Value parameters = Value(map);
    if(uploaded)
        sendMessageWithParams("saveLevel", parameters);
    else
        sendMessageWithParams("newLevel", parameters);
    uploaded = true;
}

void Level::clear(){
    map.erase("data");
}

void Level::entitiesToMap(std::vector<Entity*> entities){
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* map = doc.NewElement("Map");
    for(int x = 0; x < entities.size(); x++){
        if(entities[x] != NULL){
            tinyxml2::XMLElement* entity = doc.NewElement("entity");
            entity->SetAttribute("type", TAG_TO_TYPE(entities[x]->getTag()));
            entity->SetAttribute("x", entities[x]->getPosition().x);
            entity->SetAttribute("y", entities[x]->getPosition().y);
            entity->SetAttribute("width", entities[x]->getSize().x);
            entity->SetAttribute("height", entities[x]->getSize().y);
            entity->SetAttribute("xVelocity", entities[x]->getLaunchVelocity().x);
            entity->SetAttribute("yVelocity", entities[x]->getLaunchVelocity().y);
            map->InsertFirstChild(entity);
        }
    }
    doc.InsertFirstChild(map);
    tinyxml2::XMLPrinter printer;
    doc.Print(&printer);
    this->map["data"] = printer.CStr();
}

std::vector<Entity*> Level::mapToEntities(){
    std::vector<Entity*> ents;
    tinyxml2::XMLDocument doc;
    doc.Parse(map["data"].asString().c_str());
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
            ents.push_back(Entity::createEntity(tempType, tempPos, tempSize, tempVelocity));
        } while(iterEnt->NextSiblingElement());
    }
    return ents;
}

std::string Level::getName(){
    return map["name"].asString();
}

std::string Level::getAuthor(){
    return map["author"].asString();
}

int Level::getNumFavorites(){
    return std::atoi(map["favorites"].asString().c_str());
}

bool Level::getPublic(){
    return (map["status"].asString().compare("Public") == 0);
}

void Level::setPublic(bool isPublic){
    if(isPublic)
        map["status"] = "Public";
    else
        map["status"] = "Private";
}

bool Level::getIsFavorited(){
    if(map["favorited"].asString().compare("true") == 0)
        return true;
    return false;
}

void Level::setName(std::string n){
    map["name"] = n;
}

void Level::setMap(ValueMap map){
    this->map = map;
}