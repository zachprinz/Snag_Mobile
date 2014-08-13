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
    Entity* ent = new Entity(Vec2(pos.x + (size.x/2.0), pos.y - (size.y/2.0)), size, Vec2(0,0), WALL);
    return ent;
}
Entity* Entity::createSpikeWall(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(Vec2(pos.x + size.x/2.0, pos.y - size.y/2.0), size, Vec2(0,0), SPIKE_WALL);
    return ent;
}
Entity* Entity::createGoal(Vec2 pos, Vec2 size){
    Entity* ent = new Entity(Vec2(pos.x + size.x/2.0, pos.y - size.y/2.0), size, Vec2(0,0), GOAL);
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
    std::string textures[6] = {"wall.png", "spikewall.png", "hook.png", "spawner.png", "goal.png", "user.png"};
    float baseScales[6] = {1,1,1.5,1,1,0.5};
    this->baseScale = Vec2(baseScales[type],baseScales[type]);
    this->type = type;
    this->size = size;
    this->position = pos;
    sprite = Sprite::create(textures[type]);
    sprite->retain();
    if(size.x != 0 && size.y != 0){
        baseScale.x = size.x / sprite->getBoundingBox().size.width;
        baseScale.y = size.y / sprite->getBoundingBox().size.height;
    }
    sprite->setScale(baseScale.x, baseScale.y);
    if(type != USER)
        this->SetUpPhysicsSprite(textures[type], baseScale);
}
void Entity::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    body = PhysicsBody::createBox(Size(sprite->getBoundingBox().size));
    body->setMass(PHYSICS_INFINITY);
    body->setDynamic(false);
    body->setTag(type);
    physicsSprite = Sprite::create(texture);
    physicsSprite->setPhysicsBody(body);
    physicsSprite->setPosition(position.x,position.y);
    physicsSprite->setVisible(false);
    physicsSprite->setTag(type);
    physicsSprite->retain();
    if(type == SPAWNER || type == HOOK){
        body->setContactTestBitmask(false);
        body->setCategoryBitmask(false);
    } else {
        body->setContactTestBitmask(true);
        body->setCategoryBitmask(true);
    }
}
void Entity::update(Vec2 userPosition, float boardScale){
    this->CalculateScale(userPosition, boardScale);
}
void Entity::CalculateScale(Vec2 userPosition, float boardScale){
    if(type != USER){
        position = physicsSprite->getPosition();
        float distanceToCenter = position.x - userPosition.x;
        float newDistanceToCenter = distanceToCenter / boardScale;
        float distanceToGround = 0 - position.y;
        float newDistanceToGround = distanceToGround / boardScale;
        sprite->setPosition((MainMenu::screenSize.x / 2.0) + newDistanceToCenter, 0 - newDistanceToGround);
        sprite->setScale(baseScale.x/boardScale, baseScale.y/boardScale);
    }
}
void Entity::Add(Game* game){
    sprite->setGlobalZOrder(2);
    game->addChild(sprite);
    game->addChild(physicsSprite);
}
void Entity::Remove(Game* game){
    game->removeChild(sprite);
    game->removeChild(physicsSprite);
}
Sprite* Entity::GetSprite(){
    return sprite;
}
Sprite* Entity::GetPhysicsSprite(){
    return physicsSprite;
}
Point Entity::GetPosition(){
    return position;
}
Vec2 Entity::GetLaunchVelocity(){
    return launchVelocity;
}
int Entity::GetType(){
    return type;
}
Vec2 Entity::GetSize(){
    return size;
}
