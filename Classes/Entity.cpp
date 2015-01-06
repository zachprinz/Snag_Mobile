//
//  Entity.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "Entity.h"
#include "MainMenu.h"
#include "Game.h"

USING_NS_CC;

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5

Entity* Entity::createWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0),WALL);
    return ent;
}
Entity* Entity::createSpikeWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0),SPIKE_WALL);
    return ent;
}
Entity* Entity::createGoal(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(pos,size,Vec2(0,0),GOAL);
    return ent;
}
Entity* Entity::createHook(Vec2 pos){
    Entity* ent = new Entity(pos, Vec2(0,0), Vec2(0,0), HOOK);
    return ent;
}
Entity* Entity::createSpawner(Vec2 pos, Vec2 vel){
    Entity* ent = new Entity(pos, Vec2(0,0), vel, SPAWNER);
    return ent;
}
int Entity::count = 0;

Entity::Entity(Vec2 pos, Vec2 size, Vec2 vel, int type){
    ID = count++;
    std::string textures[6] = {"wall.png", "spikewall.png", "game_hook.png", "spawner.png", "goal.png", "user.png"};
    float baseScales[6] = {1,1,1,1,1,1};
    this->baseScale = Vec2(baseScales[type],baseScales[type]);
    this->type = type;
    this->size = Vec2(size.x,size.y);
    this->position = Vec2(pos.x, pos.y);
    sprite = Sprite::create(textures[type]);
    this->originalSize = Vec2(sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->retain();
    sprite->setGlobalZOrder(0);
    if(this->size.x != 0 && this->size.y != 0){
        baseScale.x = this->size.x / sprite->getBoundingBox().size.width;
        baseScale.y = this->size.y / sprite->getBoundingBox().size.height;
    }
    this->launchVelocity = vel;
    sprite->setScale(baseScale.x, baseScale.y);
    if(type != USER)
        this->SetUpPhysicsSprite(textures[type], baseScale);
    sprite->setPosition(position);
    if(type == HOOK){
        line = Sprite::create("game_line.png");
        line->retain();
        line->setAnchorPoint(Vec2(0.0,0.5));
        line->setVisible(false);
        line->setPosition(this->position.x,this->position.y);
    }
}
void Entity::SetLine(float size, float angle){
    if(type == HOOK){
        line->setRotation(0);
        line->setScale((size * 2)/100,5);
        line->setRotation(angle);
        line->setVisible(true);
    }
}
void Entity::SetLineOff(){
    if(type == HOOK){
        line->setVisible(false);
    }
}
void Entity::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    auto physMat = PhysicsMaterial(0.1,1,0);
    body = PhysicsBody::createBox(Size(sprite->getBoundingBox().size.width, sprite->getBoundingBox().size.height),physMat);
    body->setMass(PHYSICS_INFINITY);
    body->setDynamic(false);
    body->setTag(type);
    sprite->setPhysicsBody(body);
    sprite->setTag(type);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    if(type == SPAWNER || type == HOOK){
        body->setContactTestBitmask(false);
        body->setCategoryBitmask(false);
    } else {
        body->setContactTestBitmask(true);
        body->setCategoryBitmask(true);
    }
}
void Entity::update(Vec2 userPosition, float boardScale){
    
}
void Entity::CalculateScale(Vec2 userPosition, float boardScale){

}
void Entity::Add(Game* game){
    sprite->setGlobalZOrder(0);
    game->layers[type]->addChild(sprite,1);
    if(type == HOOK){
        game->layers[type]->addChild(line,1);
    }
    if(type == USER){
        game->layers[HOOK]->addChild(((User*)this)->line,1);
    }
}
void Entity::Remove(Game* game){
    this->sprite->removeFromParent();
    if(type == HOOK){
        line->removeFromParent();
    }
    if(type == USER){
        //game->layers[HOOK]->addChild(((User*)this)->line,1);
    }
    //game->removeChild(sprite);
}
Sprite* Entity::GetSprite(){
    return sprite;
}
Point Entity::GetPosition(){
    return position;
}
void Entity::SetPosition(Vec2 pos){
    position = pos;
}
Vec2 Entity::GetLaunchVelocity(){
    return launchVelocity;
}
void Entity::SetLaunchVelocity(Vec2 vl){
    launchVelocity = vl;
}
int Entity::GetType(){
    return type;
}
Vec2 Entity::GetSize(){
    return size;
}
void Entity::SetSize(Vec2 size){
    this->size = size;
}
Vec2 Entity::GetOriginalSize(){
    return originalSize;
}
