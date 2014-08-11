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
#include "Spawner.h"
#include "Level.h"

#define FALL_SPEED 2500

int User::type;

User::User() : Entity(Vec2(150,150), Vec2(0,0), Vec2(0,0), 5){
    SetUpPhysicsSprite("user.png", Vec2(0.5,0.5));
    isHooked = false;
    line = Sprite::create("line.png");
    line->setAnchorPoint(Vec2(0,0.5));
    line->setPositionZ(-1);
}
void User::SetUpPhysicsSprite(std::string texture, Vec2 scale){
    body = PhysicsBody::createCircle(sprite->getBoundingBox().size.width / 2.0);
    body->setMass(10.0f);
    body->setDynamic(true);
    body->setTag(type);
    physicsSprite = Sprite::create(texture);
    physicsSprite->setPhysicsBody(body);
    physicsSprite->setPosition(position.x,position.y);
    physicsSprite->setVisible(false);
    physicsSprite->setTag(type);
    physicsSprite->retain();
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
}
void User::Add(Layer* layer){
    layer->addChild(sprite);
    layer->addChild(physicsSprite);
    layer->addChild(line);
}
void User::update(float boardScale){
    if(position.y < -400)
        Reset();
    this->CalculateScale(Vec2(0,0), boardScale);
}
void User::Snag(){
    closest = Game::Instance->GetClosestHook(position);
    isHooked = true;
    line->setVisible(true);
    joint = PhysicsJointDistance::construct(physicsSprite->getPhysicsBody(), closest->GetPhysicsSprite()->getPhysicsBody(), Vec2(0.5,0.5), Vec2(0.5,0.5));
    joint->setEnable(true);
    Game::Instance->AddJoint(joint);
    lineBaseScale = joint->getDistance() / line->getTexture()->getPixelsWide();
}
void User::Release(){
    if(isHooked == true){
        isHooked = false;
        line->setVisible(false);
        Game::Instance->RemoveAllJoints();
        body->setVelocity(Vec2(body->getVelocity().x * 1.25, body->getVelocity().y * 1.25));
    }
}
void User::Reset(){
    if(isHooked)
        Release();
    Game::Instance->time = 0.0;
    physicsSprite->setPosition(Game::Instance->currentLevel->GetLaunchPosition());
    body->setVelocity(Game::Instance->currentLevel->GetLaunchVelocity());
}
void User::CalculateScale(Vec2 userPosition, float boardScale){
    position = physicsSprite->getPosition();
    float distanceToGround = 0 - position.y;
    float newDistanceToGround = distanceToGround / boardScale;
    sprite->setPosition((MainMenu::screenSize.x / 2.0), 0 - newDistanceToGround);
    sprite->setScale((baseScale.x/boardScale),(baseScale.y/boardScale));
    printf("\nUser Position: (%f, %f)", sprite->getPosition().x, sprite->getPosition().y);
    if(isHooked){
        line->setPosition(closest->GetSprite()->getPosition());
        line->setScale(lineBaseScale*(1/boardScale), 10.0 * (1/boardScale));
        line->setRotation(GetAngle(joint->getBodyA()->getPosition(),joint->getBodyB()->getPosition()));
    }
}
void User::Bounce(const PhysicsContactData* bounceData){
    if(isHooked)
        Release();
    std::vector<Vec2> points;
    for(int x = 0; x < PhysicsContactData::POINT_MAX; x++){
        points.push_back(bounceData->points[x]);
    }
    Vec2 relativePoint = Vec2(points[0].x - position.x, points[0].y - position.y);
    if(abs(relativePoint.x) < 30){
        if(abs(backupVelocity.y) > 30)
            body->setVelocity(Vec2(backupVelocity.x * 0.9, backupVelocity.y * -0.9));
    }
    else
        body->setVelocity(Vec2(backupVelocity.x * -0.9, backupVelocity.y * 1.15));
}
void User::SetBackupVelocity(){
    backupVelocity = physicsSprite->getPhysicsBody()->getVelocity();
}
Point User::GetPhysicalPosition(){
    return physicsSprite->getPosition();
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