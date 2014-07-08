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
#include <iomanip>
#include "MainMenu.h"

USING_NS_CC;

Board* Board::Instance;
char* Board::levelPath;
bool Board::customLevel;
std::vector<Level*> Board::myLevels;
std::vector<Level*> Board::localLevels;
std::vector<Level*> Board::onlineLevels;

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
    
    timeLabel = Label::createWithBMFont("dimbo.fnt", "0.00", TextHAlignment::LEFT);
    timeLabel->setPosition(visibleSize.width / 2.0 - (80 * MainMenu::screenScale.x), visibleSize.height);
    timeLabel->setAnchorPoint(Point(0.0,1.0));
    timeLabel->setPositionZ(4);
    
    scale = 1.0f;
    user = new User();
    gravity.set(0,-1);
    
    currentLevel = NULL;
}

void Board::SetPhysicsWorld(PhysicsWorld* world){
    this->world = world;
    this->world->setGravity(Vec2(0,-300));
}

void Board::UpdateTimer(float dt){
    time += 0.1;
    std::ostringstream out;
    int prec = 2;
    if(((int)time) % 10 >= 1)
        prec = 3;
    if(((int)time) % 100 >= 1)
        prec = 4;
    out << std::setprecision(prec) << time;
    if(fmod(std::atof(out.str().c_str()),1.0) == 0.0)
        out << ".0";
    timeLabel->setString(out.str());
}

void Board::onContactPostSolve(PhysicsContact& contact){
    int type = contact.getShapeB()->getBody()->getTag();
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
    if(type == -1){
        Print("You've reached the goal zone! You Win!");
    }
}

bool Board::onContactBegin(PhysicsContact& contact){
    if(contact.getShapeB()->getBody()->getTag() == -1){
        onWin();
        return false;
    }
    user->SetBackupVelocity(user->body->getVelocity());
    return true;
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
void Board::onTouchEnd(){
    log("Recognised end of touch.");
    user->Release();
}
void Board::onTouchBegin(){
    log("Recognised touch.");
    user->Snag();
}
void Board::onWin(){
    Print("You Win!");
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
    for(int x = 0; x < currentLevel->ents.size(); x++){
        if(currentLevel->ents[x]->type == 2){
            float distance = hooks[x]->GetPosition().getDistance(pos);
            if(distance <= smallestDistance){
                smallestDistance = distance;
                closestHook = hooks[x];
            }
        }
    }
    return closestHook;
}
void Board::Reset(Level* lvl){
    if(currentLevel != NULL)
        Clear();
    LoadLevel(lvl);
    user->Reset();
}
void Board::Clear(){
    for(int x = 0; x < currentLevel->ents.size(); x++){
        game->removeChild(currentLevel->ents[x]->GetSprite());
        game->removeChild(currentLevel->ents[x]->physicsSprite);
    }
}
void Board::PrintVec2(std::string name, Vec2 vec){
    //std::cout << name + ".x: " << std::to_string(vec.x) << " " + name + ".y: " << std::to_string(vec.y) << std::endl;
}
void Board::Print(std::string message){
    std::cout << message << std::endl;
}
void Board::LoadLevel(Level* lvl){
    currentLevel = lvl;
    for(int x = 0; x < currentLevel->ents.size(); x++){
        if(currentLevel->ents[x]->GetSprite() != NULL)
            game->addChild(currentLevel->ents[x]->GetSprite());
        if(currentLevel->ents[x]->physicsSprite != NULL)
            game->addChild(currentLevel->ents[x]->physicsSprite);
    }
}