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

int User::type;

User::User() : Entity(Vec2(150,150), Vec2(0,0), Vec2(0,0), 5){
    SetUpPhysicsSprite("user.png", Vec2(1.0,1.0));
    isHooked = false;
    line = Sprite::create("game_line.png");
    line->retain();
    line->setAnchorPoint(Vec2(0.0,0.5));
    line->setVisible(false);
    sprite->setGlobalZOrder(2);
}
void User::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    auto physMat = PhysicsMaterial(2,1,0);
    body = PhysicsBody::createCircle(sprite->getBoundingBox().size.width / (2.0),physMat);
    body->setMass(10.0f);
    body->setDynamic(true);
    body->setTag(type);
    sprite->setPhysicsBody(body);
    sprite->setPosition(position.x,position.y);
    sprite->setAnchorPoint(Vec2(0.5,0.5));
    sprite->setTag(type);
    body->setVelocityLimit(500);
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
}
void User::Add(Game* layer){
    sprite->setGlobalZOrder(0);
    line->setGlobalZOrder(-1);
    layer->layers[HOOK]->addChild(line,1);
    layer->layers[5]->addChild(sprite,1);
}
void User::update(float boardScale){
    if(position.y < -400)
        Reset();
    this->CalculateScale(Vec2(0,0), boardScale);
}
void User::Snag(){
    closest = Game::Instance->GetClosestHook(position);
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
    body->setVelocity(Vec2(300.f, 300.f));//oVel.x, oVel.y));
}
void User::CalculateScale(Vec2 userPosition, float boardScale){
    position = Vec2(sprite->getPosition().x, sprite->getPosition().y);
    printf("\n User Velocity: %f, %f", body->getVelocity().x, body->getVelocity().y);
    if(isHooked){
        closest->SetLine(lineBaseScale,GetAngle(joint->getBodyA()->getPosition(), joint->getBodyB()->getPosition()));
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
float User::GetAngle(Vec2 a, Vec2 b){
    float o = a.x - b.x;
    float adj = a.y - b.y;
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
