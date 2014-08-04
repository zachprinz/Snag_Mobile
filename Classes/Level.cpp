//
//  Level.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#include "Level.h"
#include "tinyxml2.h"

Level::Level(std::string path, std::string name){
    this->name = name;
    this->path = path;
    createdMapObjects = false;
}
Level* Level::createWithXML(std::string path){
    tinyxml2::XMLDocument doc;
    doc.LoadFile(path.c_str());
    std::string name = doc.RootElement()->Attribute("name");
    
    Level* level = new Level(path, name);
    
    tinyxml2::XMLElement* hooksNode = doc.RootElement()->FirstChildElement();
    tinyxml2::XMLElement* currentHook = hooksNode->FirstChildElement();
    while(currentHook != NULL){
        int x;
        int y;
        currentHook->QueryIntAttribute("x", &x);
        currentHook->QueryIntAttribute("y", &y);
        level->AddEntity(new Hook(Vec2(x,y)));
        currentHook = currentHook->NextSiblingElement();
    }
    tinyxml2::XMLElement* wallsNode = doc.RootElement()->FirstChildElement("Walls");
    tinyxml2::XMLElement* currentWall = wallsNode->FirstChildElement();
    while(currentWall != NULL){
        int x;
        int y;
        int height;
        int width;
        currentWall->QueryIntAttribute("x", &x);
        currentWall->QueryIntAttribute("y", &y);
        currentWall->QueryIntAttribute("width", &width);
        currentWall->QueryIntAttribute("height", &height);
        level->AddEntity(new Wall(Vec2(x,y),Vec2(width,height)));
        currentWall = currentWall->NextSiblingElement();
    }
    tinyxml2::XMLElement* SpikeWallsNode = doc.RootElement()->FirstChildElement("SpikeWalls");
    tinyxml2::XMLElement* currentSpikeWall = SpikeWallsNode->FirstChildElement();
    tinyxml2::XMLElement* GoalsNode = doc.RootElement()->FirstChildElement("Goals");
    tinyxml2::XMLElement* currentGoal = GoalsNode->FirstChildElement();
    tinyxml2::XMLElement* SpawnersNode = doc.RootElement()->FirstChildElement("Spawners");
    tinyxml2::XMLElement* currentSpawner = SpawnersNode->FirstChildElement();
    bool hasSpikewall = false;
    while(currentSpikeWall != NULL){
        int x;
        int y;
        int height;
        int width;
        currentSpikeWall->QueryIntAttribute("x", &x);
        currentSpikeWall->QueryIntAttribute("y", &y);
        currentSpikeWall->QueryIntAttribute("width", &width);
        currentSpikeWall->QueryIntAttribute("height", &height);
        level->AddEntity(new SpikeWall(Vec2(x,y),Vec2(width,height)));
        currentSpikeWall = currentSpikeWall->NextSiblingElement();
        hasSpikewall = true;
    }
    while(currentGoal != NULL){
        int x;
        int y;
        int height;
        int width;
        currentGoal->QueryIntAttribute("x", &x);
        currentGoal->QueryIntAttribute("y", &y);
        currentGoal->QueryIntAttribute("width", &width);
        currentGoal->QueryIntAttribute("height", &height);
        level->AddEntity(new Goal(Vec2(x,y),Vec2(width,height)));
        currentGoal = currentGoal->NextSiblingElement();
    }
    while(currentSpawner != NULL){
        int x;
        int y;
        int xVelocity;
        int yVelocity;
        currentSpawner->QueryIntAttribute("x", &x);
        currentSpawner->QueryIntAttribute("y", &y);
        currentSpawner->QueryIntAttribute("xVelocity", &xVelocity);
        currentSpawner->QueryIntAttribute("yVelocity", &yVelocity);
        level->AddEntity(new Spawner(Vec2(x,y),Vec2(xVelocity,yVelocity)));
        currentSpawner = currentSpawner->NextSiblingElement();
    }
    if(!hasSpikewall){
        level->AddEntity(new SpikeWall(Vec2(0,-100), Vec2(1,1)));
    }
    return level;
};
Level* Level::createWithMapObjects(std::vector<MapObject*> mapObjects, std::string name){
    std::string path = FileUtils::getInstance()->getWritablePath() + name + ".xml";
    
    // Add the newly created level to the MyLevels document //
    // Doesn't work because we cannot write a file in the resources folder //
    tinyxml2::XMLDocument myLevelsDoc;
    myLevelsDoc.LoadFile(FileUtils::getInstance()->fullPathForFilename("MyLevels.xml").c_str());
    tinyxml2::XMLElement* newLevel = myLevelsDoc.NewElement("level");
    newLevel->SetAttribute("name", name.c_str());
    newLevel->SetAttribute("path", path.c_str());
    myLevelsDoc.InsertEndChild(newLevel);
    // End //
    
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* walls = doc.NewElement("Walls");
    tinyxml2::XMLElement* spawners = doc.NewElement("Spawners");
    tinyxml2::XMLElement* spikewalls = doc.NewElement("SpikeWalls");
    tinyxml2::XMLElement* hooks = doc.NewElement("Hooks");
    tinyxml2::XMLElement* map = doc.NewElement("map");
    tinyxml2::XMLElement* goals = doc.NewElement("Goals");
    map->SetAttribute("name", name.c_str());
    for(int x = 0; x < mapObjects.size(); x++){
        switch(mapObjects[x]->GetType()){
            case WALL:{
                tinyxml2::XMLElement* wall = doc.NewElement("wall");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("width", mapObjects[x]->GetSize().x);
                wall->SetAttribute("height", mapObjects[x]->GetSize().y);
                walls->InsertEndChild(wall);
                break;
            }
            case GOAL:{
                tinyxml2::XMLElement* wall = doc.NewElement("goal");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("width", mapObjects[x]->GetSize().x);
                wall->SetAttribute("height", mapObjects[x]->GetSize().y);
                goals->InsertEndChild(wall);
                break;
            }
            case SPIKE_WALL:{
                tinyxml2::XMLElement* wall = doc.NewElement("spikewall");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("width", mapObjects[x]->GetSize().x);
                wall->SetAttribute("height", mapObjects[x]->GetSize().y);
                spikewalls->InsertEndChild(wall);
                break;
            }
            case SPAWNER:{
                tinyxml2::XMLElement* wall = doc.NewElement("spawner");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("xVelocity", 100);
                wall->SetAttribute("yVelocity", 600);
                spawners->InsertEndChild(wall);
                break;
            }
            case HOOK:{
                tinyxml2::XMLElement* wall = doc.NewElement("hook");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                hooks->InsertEndChild(wall);
                break;
            }
        }
    }
    map->InsertFirstChild(spawners);
    map->InsertFirstChild(goals);
    map->InsertFirstChild(spikewalls);
    map->InsertFirstChild(walls);
    map->InsertFirstChild(hooks);
    doc.InsertFirstChild(map);
    
    doc.SaveFile(path.c_str());
    
    Level* level = Level::createWithXML(path);
    level->SetName(name);
    level->SetMapObjects(mapObjects);
    return level;
};
void Level::SetMapObjects(std::vector<MapObject*> objs){
    this->mapObjects = objs;
    createdMapObjects = true;
}
std::string Level::GetPath(){
    return path;
}
std::string Level::GetName(){
    return name;
}
std::string Level::GetAuthor(){
    return "The Gaate";
    //Todo, Include author info
}
void Level::AddEntity(Entity* e){
    ents.push_back(e);
}
void Level::SetName(std::string n){
    name = n;
}
void Level::Add(Layer* game){
    for(int x = 0; x < ents.size(); x++){
        ents[x]->Add(game);
    }
}