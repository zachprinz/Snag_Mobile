#include "User.h"
#include "Game.h"
#include "MainMenu.h"
#include "Level.h"
#include "PlayScreen.h"
#include <math.h>
#include "Utils.h"

User::User() : Entity(Vec2(150,150), Vec2(0,0), Vec2(0,0), "user.png", true){
    setUpPhysicsSprite(true);
    
    sprite->setGlobalZOrder(2);
    sprite->setPosition(getPosition().x,getPosition().y);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    
    hooked = false;
    
    line = Sprite::create("game_line.png");
    line->retain();
    line->setAnchorPoint(Vec2(0.0,0.5));
    line->setVisible(false);
    
    focusPointSprite = Sprite::create("user.png");
    focusPointSprite->retain();
    focusPointSprite->setScale(0.4,0.4);
    focusPointSprite->setAnchorPoint(Vec2(0.5,0.5));
    targetFocusPointSprite = Sprite::create("user.png");
    targetFocusPointSprite->retain();
    targetFocusPointSprite->setScale(0.4,0.4);
    targetFocusPointSprite->setAnchorPoint(Vec2(0.5,0.5));
    targetFocusPointSprite->setColor(Color3B(0,0,0));
    
    setTag(USER);
    
    tailCount = 5;
    for(int x = 0; x < tailCount; x++){
        pastSprites.push_back(Sprite::create("user.png"));
        pastSprites[x]->setPosition(getPosition().x,getPosition().y);
        pastSprites[x]->retain();
        pastSprites[x]->setAnchorPoint(Vec2(0.5,0.5));
        pastSprites[x]->setVisible(true);
        pastSprites[x]->setOpacity(240);
        pastSprites[x]->setTag(getTag());
    }
    
    line = Sprite::create("game_line.png");
    line->retain();
    line->setAnchorPoint(Vec2(0.0,0.5));
    line->setVisible(false);
    line->setPosition(getPosition().x,getPosition().y);
    line->setTag(LINE);
    
}

void User::setUpPhysicsSprite(bool collision){
    body = PhysicsBody::createCircle(sprite->getBoundingBox().size.width / (2.0), PhysicsMaterial(2,1,0));
    body->setMass(10.0f);
    body->setDynamic(true);
    body->setVelocityLimit(500);
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
    body->setRotationEnable(false);
    sprite->setPhysicsBody(body);
}

void User::add(){
    sprite->setGlobalZOrder(0);
    line->setGlobalZOrder(-1);
    PlayScreen::Instance->addSprite(line);
    PlayScreen::Instance->addSprite(sprite);
    for(int x = 0; x < sprites.size(); x++)
        PlayScreen::Instance->addSprite(sprites[x]);
    line->release();

    //layer->layers[5]->addChild(focusPointSprite,1);
    //layer->layers[5]->addChild(targetFocusPointSprite,1);
}

void User::remove(){
    PlayScreen::Instance->removeChild(line);
    PlayScreen::Instance->removeChild(sprite);
    line->retain();
    line->removeFromParent();
}

void User::setLine(float size, float angle){
    line->setRotation(0);
    line->setScale((size)/100,5);
    line->setRotation(angle);
    line->setVisible(true);
}

void User::setLineOff(){
    line->setVisible(false);
}

void User::update(){
    updatePosition();
    updateSpriteStretch();
}

void User::setStretch(Sprite* sprite, float angle, float magnitude){
    sprite->setRotation(0);
    sprite->setScaleY(magnitude * 0.5);
    sprite->setScaleX(1/magnitude * 0.5);
    sprite->setRotation(angle);
}

void User::snag(){
    closest = PlayScreen::Instance->getGame()->getClosestHook(getPosition());
    closestPosition = closest->getPosition();
    hooked = true;
    joint = PhysicsJointDistance::construct(body, closest->getPhysicsBody(), Vec2(0.5,0.5), Vec2(0.5,0.5));
    joint->setCollisionEnable(false);
    joint->setEnable(true);
    PlayScreen::Instance->addJoint(joint);
    lineBaseScale = (joint->getDistance());
}

void User::release(bool add){
    if(isHooked() == true){
        hooked = false;
        setLineOff();
        PlayScreen::Instance->removeAllJoints();
        if(add)
            body->setVelocity(Vec2(body->getVelocity().x * 1.35, body->getVelocity().y * 1.25));
    }
}

void User::reset(Vec2 launchPosition, Vec2 launchVelocity){
    if(isHooked())
        release();
    sprite->setPosition(launchPosition);
    body->setVelocity(launchVelocity);
    body->resetForces();
    for(int x = 0;x < sprites.size(); x++){
        Vec2 userPos;
        switch (x) {
            case 0:
                userPos = Vec2(0.5,0);
                break;
            case 1:
                userPos = Vec2(0,0.5);
                break;
            case 2:
                userPos = Vec2(-0.5,0);
                break;
            case 3:
                userPos = Vec2(0,-0.5);
                break;
        }
        sprites[x]->setPosition(body->getPosition().x + (userPos.x * 60), body->getPosition().y + (userPos.y *60));
    }
}

void User::updatePosition(){
    if(isHooked()){
        float distance = getPosition().getDistance(closest->getPosition());
        setLine(distance,getAngle(joint->getBodyA()->getPosition(), joint->getBodyB()->getPosition()));
    }
    //focusPointSprite->setPosition(Game::Instance->focusPoint);
    //targetFocusPointSprite->setPosition(Game::Instance->targetFocusPoint);
}

void User::updateSpriteStretch(){
    float angle, scale;
    Vec2 vel = sprite->getPhysicsBody()->getVelocity();
    angle = getAngle(Vec2(0,0),vel);
    float velMag = sqrtf(pow(vel.x,2) + pow(vel.y,2));
    if(velMag == 0)
        velMag = 1;
    scale = 1/((std::log10((velMag/31)+1)));
    if(scale > 1.0)
        scale = 1.0;
    setStretch(sprite, angle, scale);
}

void User::Bounce(const PhysicsContactData* bounceData){
    if(isHooked())
        release(false);
}

Vec2 User::getClosestPosition(){
    if(closest != nullptr){
        if(isHooked())
            return closestPosition;
        return getPosition();
    }
    return Vec2(0,0);
}
