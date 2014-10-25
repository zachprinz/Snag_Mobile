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
    line->setGlobalZOrder(1);
    line->setVisible(false);
    sprite->setGlobalZOrder(2);
}
void User::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    body = PhysicsBody::createCircle(sprite->getBoundingBox().size.width / (PTM_RATIO * 2.0));
    body->setMass(1.0f);
    body->setDynamic(true);
    body->setTag(type);
    physicsSprite = Sprite::create(texture);
    physicsSprite->setPhysicsBody(body);
    physicsSprite->setPosition(position.x/PTM_RATIO,position.y/PTM_RATIO);
    //physicsSprite->setScale((baseScale.x*physicsSprite->getBoundingBox().size.width)/PTM_RATIO, (baseScale.y * physicsSprite->getBoundingBox().size.height)/PTM_RATIO);
    physicsSprite->setAnchorPoint(Vec2(0.5,0.5));
    physicsSprite->setVisible(false);
    physicsSprite->setTag(type);
    physicsSprite->retain();
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
}
void User::Add(Layer* layer){
    sprite->setGlobalZOrder(0);
    line->setGlobalZOrder(-1);
    layer->addChild(line,1);
    layer->addChild(sprite,1);
    layer->addChild(physicsSprite,1);
}
void User::update(float boardScale){
    printf("\nUser Physics Position: (%f, %f)", physicsSprite->getPosition().x, physicsSprite->getPosition().y);
    if(position.y < -400)
        Reset();
    this->CalculateScale(Vec2(0,0), boardScale);
}
void User::Snag(){
    closest = Game::Instance->GetClosestHook(position);
    isHooked = true;
    //line->setVisible(true);
    joint = PhysicsJointDistance::construct(body, closest->GetPhysicsSprite()->getPhysicsBody(), Vec2(0.5,0.5), Vec2(0.5,0.5));
    joint->setCollisionEnable(false);
    joint->setDistance(physicsSprite->getPosition().getDistance(closest->GetPhysicsSprite()->getPosition()));
    joint->setEnable(true);
    Game::Instance->AddJoint(joint);
    //line->setScale(1.0,1.0);
    lineBaseScale = (joint->getDistance()*PTM_RATIO) / 100.0;//line->getBoundingBox().size.width;
}
void User::Release(){
    if(isHooked == true){
        isHooked = false;
        line->setVisible(false);
        Game::Instance->RemoveAllJoints();
        body->setVelocity(Vec2(body->getVelocity().x * 1.35, body->getVelocity().y * 1.25));
    }
}
void User::Reset(){
    if(isHooked)
        Release();
    Game::Instance->time = 0.0;
    Vec2 oLaunch = Game::Instance->currentLevel->GetLaunchPosition();
    Vec2 oVel = Game::Instance->currentLevel->GetLaunchVelocity();
    physicsSprite->setPosition(Vec2(oLaunch.x / PTM_RATIO, oLaunch.y / PTM_RATIO));
    body->setVelocity(Vec2(200.f, 250.f));//oVel.x, oVel.y));
}
void User::CalculateScale(Vec2 userPosition, float boardScale){
    position = Vec2(physicsSprite->getPosition().x * PTM_RATIO, physicsSprite->getPosition().y * PTM_RATIO);
    float distanceToGround = 0 - position.y;
    float newDistanceToGround = distanceToGround / boardScale;
    sprite->setPosition((MainMenu::screenSize.x / 2.0), 0 - newDistanceToGround);
    sprite->setScale((baseScale.x/boardScale),(baseScale.y/boardScale));
    if(isHooked){
        line->setPosition(closest->GetSprite()->getPosition());
        line->setScale(lineBaseScale/boardScale, 10.0/boardScale);
        line->setRotation(GetAngle(joint->getBodyA()->getPosition(),joint->getBodyB()->getPosition()));
        line->setVisible(true);
    }
}
void User::Bounce(const PhysicsContactData* bounceData){
    if(isHooked)
        Release();
    std::vector<Vec2> points;
    for(int x = 0; x < PhysicsContactData::POINT_MAX; x++){
        points.push_back(bounceData->points[x]);
    }
    Vec2 relativePoint = Vec2(fabs(points[0].x - (position.x/PTM_RATIO)), fabs(points[0].y - (position.y/PTM_RATIO)));
    if(relativePoint.x < relativePoint.y){
        body->setVelocity(Vec2(backupVelocity.x * 0.95, backupVelocity.y * -0.9));
    } else
        body->setVelocity(Vec2(backupVelocity.x * -0.95, backupVelocity.y * 1.1));
}
void User::SetBackupVelocity(){
    backupVelocity = physicsSprite->getPhysicsBody()->getVelocity();
}
Point User::GetPhysicalPosition(){
    return Vec2(physicsSprite->getPosition().x*PTM_RATIO, physicsSprite->getPosition().y * PTM_RATIO);
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
