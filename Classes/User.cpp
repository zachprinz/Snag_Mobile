//
//  User.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "User.h"
#include "Board.h"
#include "Spawner.h"

#define FALL_SPEED 2500

int User::type;

User::User() : Entity("user.png"){
    position.set(150,150);
    SetBaseScale(Vec2(0.5,0.5));
    isHooked = false;
    boardScale = 1.0f;
    userPosition = Vec2(150,150);
    line = Sprite::create("line.png");
    line->setAnchorPoint(Vec2(0,0.5));
    line->setPositionZ(-1);
    Board::Instance->game->addChild(line);
    SetUpPhysicsSprite("user.png");
    type = 5;
}

void User::SetUpPhysicsSprite(char* texture){
    physicsSprite = Sprite::create(texture);
    physicsSprite->setTag(5);
    body = PhysicsBody::createCircle(imageSize.x * spriteBaseScale.x * 0.5);
    body->setVelocity(Vec2(100,300));//Vec2(Spawner::Instance->GetVelocity().x,Spawner::Instance->GetVelocity().y));
    body->setMass(10.0f);
    body->setDynamic(true);
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
    //body->setCollisionBitmask(true);
    physicsSprite->setPhysicsBody(body);
    physicsSprite->setPosition(position.x,position.y);
    physicsSprite->setVisible(false);
    physicsSprite->setScale(spriteBaseScale.x,spriteBaseScale.y);
    Board::Instance->game->addChild(physicsSprite);
}

void User::update(float dt){
    if(position.y < -400)//(abs(body->getVelocity().x) < 0.1 && abs(body->getVelocity().y) < 0.1) || position.y < -200)
        Reset();
    userPosition = position;
    Board::PrintVec2("UserPosition", GetBounds().origin);
}

void User::Snag(){
    closest = Board::Instance->GetClosestHook(position);
    isHooked = true;
    line->setVisible(true);
    joint = PhysicsJointDistance::construct(physicsSprite->getPhysicsBody(), closest->physicsSprite->getPhysicsBody(), Vec2(0.5,0.5), Vec2(0.5,0.5));
    joint->setEnable(true);
    Board::Instance->AddJoint(joint);
    lineBaseScale = joint->getDistance() / line->getTexture()->getPixelsWide();
}

void User::Release(){
    if(isHooked == true){
        isHooked = false;
        line->setVisible(false);
        Board::Instance->world->removeAllJoints();
        body->setVelocity(Vec2(body->getVelocity().x * 1.25, body->getVelocity().y * 1.25));
    }
}

void User::Reset(){
    if(isHooked)
        Release();
    Board::Instance->time = 0.0;
    physicsSprite->setPosition(Spawner::Instance->GetPosition());
    body->setVelocity(Spawner::Instance->GetStartVelocity());
}

void User::CalculateScale(){
    position = physicsSprite->getPosition();
    float distanceToGround = 0 - position.y; // board.ground;
    float newDistanceToGround = distanceToGround / boardScale;
    sprite->setPosition((Board::Instance->visibleSize.width / 2.0), 0 -newDistanceToGround);
    //Why is this scale here?
    SetScale(Vec2((1/boardScale),(1/boardScale)));
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

void User::SetBackupVelocity(Vec2 vel){
    backupVelocity = vel;
}

Point User::GetPhysicsPosition(){
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