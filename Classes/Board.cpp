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

Board::Board(Layer* game, Size size, Point origin){
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

void Board::update(float dt){
    updateView(dt);
}
void Board::updateView(float dt){
    for(int x = 0; x < ents.size(); x++){
        ents[x]->update(dt);
    }
    float dist = (user->GetPosition().y + (user->GetSprite()->getTexture()->getPixelsHigh() * 0.5 * scale) - 0);
    if(dist < visibleSize.height)
        dist = visibleSize.height;
    scale = 1.0 / ((visibleSize.height - visibleSize.height * 0.15) / dist);
    Entity::boardScale = scale;
    std::cout << "Scale: " << std::to_string(scale) << std::endl;
    center.set(user->GetPosition().x, (boardSize.height / 2.0) / scale);
    for(int x = 0; x < ents.size(); x++){
        ents[x]->UpdateSprite();
    }
    user->UpdateSprite();
}
void Board::updateCollision(float dt){
    
}
Point Board::TryMove(Point destinationPoint, Rect bounds){
    Rect userRect;
    userRect.setRect(destinationPoint.x - bounds.size.width / 2.0,destinationPoint.y - bounds.size.height / 2.0,bounds.size.width,bounds.size.height);
    std::vector<Vec2> sides;
    std::vector<float> collisionPositions;
    //It's not getting any collision until it reaches the bottom
    if(CheckSpikeWallCollision(userRect)){
        user->Reset();
        return Point(0,999999999);
    }
    for(int x = 0; x < walls.size(); x++){
        if(walls[x]->GetBounds().intersectsRect(userRect)){
            std::cout << "Found Collision on";
            if(walls[x]->GetBounds().containsPoint(userRect.origin)){ //Either left or bottom;
                if(walls[x]->GetBounds().containsPoint(Point(userRect.origin.x,userRect.origin.y + userRect.size.height))){
                    sides.push_back(Vec2(-1,0));
                    collisionPositions.push_back(walls[x]->GetBounds().getMaxX());
                    std::cout << " left side." << std::endl;
                } else {
                    sides.push_back(Vec2(0,-1));
                    collisionPositions.push_back(walls[x]->GetBounds().getMaxY());
                    std::cout << " bottom side." << std::endl;
                }
            } else { // Means that the top right is intersecting so it's either top or right;
                if(walls[x]->GetBounds().containsPoint(Point(userRect.origin.x,userRect.origin.y + userRect.size.height))){
                    sides.push_back(Vec2(0,1));
                    collisionPositions.push_back(walls[x]->GetBounds().getMinY());
                    std::cout << " top side." << std::endl;
                } else {
                    sides.push_back(Vec2(1,0));
                    collisionPositions.push_back(walls[x]->GetBounds().getMinX());
                    std::cout << " right side." << std::endl;
                }
            }
        }
    }
    for(int x = 0; x < sides.size(); x++){
        if(sides[x].x != 0){
            if(sides[x].x > 0)
                destinationPoint.x -= userRect.getMaxX() - collisionPositions[x];
            else
                destinationPoint.x += collisionPositions[x] - userRect.getMinX();
        } else{
            if(sides[x].y > 0)
                destinationPoint.y -= userRect.getMaxY() - collisionPositions[x];
            else
                destinationPoint.y += collisionPositions[x] - userRect.getMinY();
        }
    }
    if(sides.size() > 0)
        user->collide(sides[0]);
    return destinationPoint;
}

bool Board::CheckSpikeWallCollision(Rect userRect){
    for(int x = 0; x < spikewalls.size(); x++){
        if(spikewalls[x]->GetBounds().intersectsRect(userRect))
            return true;
    }
    return false;
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

void Board::AddJoint(PhysicsJoint* joint){
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
void Board::LoadLevel(char* name){
    tinyxml2::XMLDocument doc;
    doc.LoadFile(FileUtils::getInstance()->fullPathForFilename("level.xml").c_str());
    
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