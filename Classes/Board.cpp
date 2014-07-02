//
//  Board.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "Board.h"
#include <string.h>
#include <iostream>
#include "tinyxml2.h"
#include <iostream>
#include <fstream>
#include "Spawner.h"

USING_NS_CC;

Board* Board::Instance;
char* Board::levelPath;

Board::Board(Layer* game, PhysicsWorld* world, Size size, Point origin){
    this->world = world;
    background = Sprite::create("background.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Vec2(0,0));
    Vec2 backgroundImageSize = Vec2(background->getTexture()->getPixelsHigh(), background->getTexture()->getPixelsWide());
    background->setScale(size.width / backgroundImageSize.y, size.height / backgroundImageSize.x);
    background->setPositionZ(-2);
    game->addChild(background);
    Instance = this;
    this->game = game;
    this->visibleSize = size;
    this->origin = origin;
    this->boardSize.setSize(3000,3000);
    
    scale = 1.0f;
    user = new User();
    LoadLevel("level.xml");
    gravity.set(0,-1);
}

void Board::SetPhysicsWorld(PhysicsWorld* world){
    this->world = world;
    this->world->setGravity(Vec2(0,-300));
}

void Board::onContactPostSolve(PhysicsContact& contact){
    int type = contact.getShapeA()->getBody()->getTag();
    int type2 = contact.getShapeB()->getBody()->getTag();
    Print( "Type: " + std::to_string(type) + " Type2: " + std::to_string(type2));

    //if(type == user->type)
        type = type2;
    if(type == Wall::type){
        Print("Contact with Wall");
        user->Bounce(contact.getContactData());
    }
    if(type == SpikeWall::type){
        Print("Contact with SpikeWall");
        user->Reset();
    }
    if(type == 3){
        Print("Contact with hook, Ignore");
    }
}

void Board::onContactBegin(PhysicsContact& contact){
    user->SetBackupVelocity(user->body->getVelocity());
}

void Board::update(float dt){
    updateView(dt);
}

void Board::updateView(float dt){
    float dist = (user->GetPhysicsPosition().y + (user->GetSprite()->getTexture()->getPixelsHigh() * 0.5 * scale) - 0);
    if(dist < visibleSize.height)
        dist = visibleSize.height;
    scale = 1.0 / ((visibleSize.height - visibleSize.height * 0.15) / dist);
    Entity::boardScale = scale;
    center.set(user->GetPosition().x, (boardSize.height / 2.0) / scale);
    user->UpdateSprite();
    user->update(dt);
    for(int x = 0; x < ents.size(); x++){
        ents[x]->UpdateSprite();
    }
    user->UpdateSprite();
}
void Board::updateCollision(float dt){
    
}
void Board::onTouchEnd(){//Touch* touch, Event* event){
    log("Recognised end of touch.");
    user->Release();
}
void Board::onTouchBegin(){//Touch* touch, Event* event){
    log("Recognised touch.");
    user->Snag();
}

void Board::AddEntity(Entity* ent){
    game->addChild(ent->GetSprite());
    ents.push_back(ent);
}

void Board::AddHook(Hook* hook){
    hooks.push_back(hook);
    ents.push_back(hook);
}

void Board::AddSpikeWall(SpikeWall* sw){
    spikewalls.push_back(sw);
    ents.push_back(sw);
}

void Board::AddWall(Wall* wall){
    walls.push_back(wall);
    ents.push_back(wall);
}

void Board::AddSpawner(Spawner* spawner){
    spawners.push_back(spawner);
    ents.push_back(spawner);
}

void Board::AddJoint(PhysicsJointDistance* joint){
    world->addJoint(joint);
}
void Board::RemoveJoint(){
    world->removeAllJoints();
}

Hook* Board::GetClosestHook(Point pos){
    float smallestDistance = 9999;
    if(hooks.size() > 0) smallestDistance = hooks[0]->GetPosition().getDistance(pos);
    Hook* closestHook;
    for(int x = 0; x < hooks.size(); x++){
        float distance = hooks[x]->GetPosition().getDistance(pos);
        if(distance <= smallestDistance){
            smallestDistance = distance;
            closestHook = hooks[x];
        }
    }
    return closestHook;
}

void Board::PrintVec2(std::string name, Vec2 vec){
    //std::cout << name + ".x: " << std::to_string(vec.x) << " " + name + ".y: " << std::to_string(vec.y) << std::endl;
}

void Board::Print(std::string message){
    std::cout << message << std::endl;
}

void Board::LoadLevel(char* name){
    tinyxml2::XMLDocument doc;
    //doc.LoadFile(FileUtils::getInstance()->fullPathForFilename("level.xml").c_str());
    if(sizeof(levelPath) / sizeof(char) > 3)
        doc.LoadFile(levelPath);
    else
        doc.LoadFile(FileUtils::getInstance()->fullPathForFilename("level.xml").c_str());
    std::cout << "FilePath: " << FileUtils::getInstance()->fullPathForFilename("level.xml").c_str() << std::endl;

    tinyxml2::XMLElement* hooksNode = doc.RootElement()->FirstChildElement();
    tinyxml2::XMLElement* currentHook = hooksNode->FirstChildElement();
    while(currentHook != NULL){
        int x;
        int y;
        currentHook->QueryIntAttribute("x", &x);
        currentHook->QueryIntAttribute("y", &y);
        AddHook(new Hook(Vec2(x,y)));
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
        AddWall(new Wall(Vec2(x,y),Vec2(width,height)));
        currentWall = currentWall->NextSiblingElement();
    }
    
    tinyxml2::XMLElement* SpikeWallsNode = doc.RootElement()->FirstChildElement("SpikeWalls");
    tinyxml2::XMLElement* currentSpikeWall = SpikeWallsNode->FirstChildElement();
    while(currentSpikeWall != NULL){
        int x;
        int y;
        int height;
        int width;
        currentSpikeWall->QueryIntAttribute("x", &x);
        currentSpikeWall->QueryIntAttribute("y", &y);
        currentSpikeWall->QueryIntAttribute("width", &width);
        currentSpikeWall->QueryIntAttribute("height", &height);
        AddSpikeWall(new SpikeWall(Vec2(x,y),Vec2(width,height)));
        currentSpikeWall = currentSpikeWall->NextSiblingElement();
    }
    
    tinyxml2::XMLElement* SpawnersNode = doc.RootElement()->FirstChildElement("Spawners");
    tinyxml2::XMLElement* currentSpawner = SpawnersNode->FirstChildElement();
    while(currentSpawner != NULL){
        int x;
        int y;
        int xVelocity;
        int yVelocity;
        currentSpawner->QueryIntAttribute("x", &x);
        currentSpawner->QueryIntAttribute("y", &y);
        currentSpawner->QueryIntAttribute("xVelocity", &xVelocity);
        currentSpawner->QueryIntAttribute("yVelocity", &yVelocity);
        AddSpawner(new Spawner(Vec2(x,y),Vec2(xVelocity,yVelocity)));
        currentSpawner = currentSpawner->NextSiblingElement();
    }
}