//
//  User.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "User.h"
#include "Game.h"
#include "MainMenu.h"
#include "Level.h"
#include <math.h>

int User::type;

User::User() : Entity(Vec2(150,150), Vec2(0,0), Vec2(0,0), 5){
    SetUpPhysicsSprite("user.png", Vec2(1.0,1.0));
    SetUpParticles();
    emitter->setPositionType(ParticleSystem::PositionType::GROUPED);
    emitter->setPosition(Vec2(0,0));
    isHooked = false;
    line = Sprite::create("game_line.png");
    focusPointSprite = Sprite::create("user.png");
    focusPointSprite->retain();
    focusPointSprite->setScale(0.4,0.4);
    focusPointSprite->setAnchorPoint(Vec2(0.5,0.5));
    line->retain();
    line->setAnchorPoint(Vec2(0.0,0.5));
    line->setVisible(false);
    sprite->setGlobalZOrder(2);
    body->setRotationEnable(false);
    tailCount = 5;
    for(int x = 0; x < tailCount; x++){
        pastSprites.push_back(Sprite::create("user.png"));
        pastSprites[x]->setPosition(position.x,position.y);
        pastSprites[x]->retain();
        pastSprites[x]->setAnchorPoint(Vec2(0.5,0.5));
        pastSprites[x]->setVisible(true);
        pastSprites[x]->setOpacity(240);
    }
    
    light = SpotLight::create(Vec3(0.f, -1.0f, 0.0f), Vec3(100.0f, 100.0f, 0.0f),Color3B::WHITE, 0.0, 10, 1000.0f) ;
    
    auto physMat = PhysicsMaterial(2,1,0);
    PhysicsBody* previousBody = nullptr;
    spriteUpdateCount = 0;
    printf("Getting Model\n");
    sphere = Sprite3D::create("crate.c3b");
    sphere->setScaleZ(0.5);
    sphere->retain();
    sphere->setTexture("crate_color_user.png");
    sprite->setTexture("crate_color_user.png");
    sphere->setVisible(false);
    //sphere->setTexture(sprite->getTexture());
}
void User::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    auto physMat = PhysicsMaterial(2,1,0);
    float tempSpriteSize = sprite->getBoundingBox().size.width;
    body = PhysicsBody::createCircle(tempSpriteSize / (2.0),physMat);
    body->setMass(10.0f);
    body->setDynamic(true);
    body->setTag(type);
    sprite->setPhysicsBody(body);
    sprite->setPosition(position.x,position.y);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->setScaleZ(0.25);
    sprite->setTag(type);
    body->setVelocityLimit(500);
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
}
void User::Add(Game* layer){
    sprite->setGlobalZOrder(0);
    line->setGlobalZOrder(-1);
    layer->addChild(light,1);
    //layer->addChild(line,1);
    layer->addChild(sprite,1);
    //for(int x = 0; x < sprites.size(); x++){
    //    layer->addChild(sprites[x]);
    //}
    //layer->layers[0]->addChild(sphere,1);
}
void User::update(float boardScale){
    if(position.y < -400)
        Reset();
    this->CalculateScale(Vec2(0,0), boardScale);

    float angle;
    float scale;
    Vec2 vel = sprite->getPhysicsBody()->getVelocity();
    angle = GetAngle(Vec2(0,0),vel);
    float velMag = sqrtf(pow(vel.x,2) + pow(vel.y,2));
    float over = 100;
    if(velMag == 0)
        velMag = 1;
    scale = 1/((std::log10((velMag/31)+1)));
    if(scale > 1.0)
        scale = 1.0;
    SetStretch(sprite, angle, scale);
    focusPointSprite->setPosition(Game::Instance->focusPoint);
    sphere->setPosition(sprite->getPosition());
    light->setPosition3D(sprite->getPosition3D());
}
void User::SetStretch(Sprite3D* sprite, float angle, float magnitude){
    //sprite->setRotation(0);
    //sprite->setScaleY(magnitude);
    //sprite->setScaleX(1/magnitude);
    //sprite->setRotation(angle);
}

void User::Snag(){
    closest = Game::Instance->GetClosestHook(position);
    closestPosition = closest->GetPosition();
    isHooked = true;
    joint = PhysicsJointDistance::construct(body, closest->GetSprite()->getPhysicsBody(), Vec2(0.5,0.5), Vec2(0.5,0.5));
    joint->setCollisionEnable(false);
    //joint->setDistance(sprite->getPosition().getDistance(closest->GetSprite()->getPosition()));
    joint->setEnable(true);
    Game::Instance->AddJoint(joint);
    lineBaseScale = (joint->getDistance());//line->getBoundingBox().size.width;
}
void User::Release(bool add){
    if(isHooked == true){
        isHooked = false;
        closest->SetLineOff();
        Game::Instance->RemoveAllJoints();
        if(add)
            body->setVelocity(Vec2(body->getVelocity().x * 1.35, body->getVelocity().y * 1.25));
    }
}
void User::Reset(){
    if(isHooked)
        Release();
    Game::Instance->time = 0.0;
    Vec2 oLaunch = Game::Instance->currentLevel->GetLaunchPosition();
    Vec2 oVel = Game::Instance->currentLevel->GetLaunchVelocity();
    sprite->setPosition(Vec2(oLaunch.x, oLaunch.y));
    body->setVelocity(Vec2(oVel.x, oVel.y));
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
    Game::Instance->focusPoint = GetPosition();
    Game::Instance->lowShift = false;
    Game::Instance->catching = false;
    Game::Instance->closing = false;
    Game::Instance->startClosingDistance = 0;
    Game::Instance->startClosingMAX_SHIFT = 30;
    Game::Instance->previousTrail = 0;
    Game::Instance->MAX_SHIFT = 30;
}
void User::CalculateScale(Vec2 userPosition, float boardScale){
    position = Vec2(sprite->getPosition().x, sprite->getPosition().y);
    if(isHooked){
        float distance = position.getDistance(closest->GetPosition());

        closest->SetLine(distance,GetAngle(joint->getBodyA()->getPosition(), joint->getBodyB()->getPosition()));
    }
}
void User::Bounce(const PhysicsContactData* bounceData){
    if(isHooked)
        Release(false);
}
void User::SetBackupVelocity(){

}
Point User::GetPhysicalPosition(){
    return Vec2(sprite->getPosition().x, sprite->getPosition().y);
}
Vec2 User::GetClosestPosition(){
    if(closest != nullptr){
        if(isHooked)
            return closestPosition;
        return GetPosition();
    }
}
float User::GetAngle(Vec2 a, Vec2 b){
    float o = a.x - b.x;
    float adj = a.y - b.y;
    if(a.x == b.x){
        if(a.y > b.y)
            return 90;
        else
            return 270;
    }
    if(a.y == b.y){
        if(a.x > b.x)
            return 0;
        else
            return 180;
    }
    float angle = atan(o/adj) * 57.29;
    if(adj < 0){ //Left two
        if(adj > 0) // bottom left
            angle = (90-angle) + 180;
        if(adj < 0) // top left
            angle = 90 + angle;
    }
    else{ // Right two
        if(adj > 0) // bottom right
            angle += 270;
        if(adj < 0) // top right
            angle = (90 - angle);
    }
    return angle;
}
