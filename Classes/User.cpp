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

User::User() : Entity("user.png"){
    SetPosition(Vec2(150,150));
    velocity.x = 500;
    velocity.y = 1500;
    SetBaseScale(Vec2(0.5,0.5));
    isHooked = false;
    boardScale = 1.0f;
    userPosition = Vec2(150,150);
    line = Sprite::create("line.png");
    line->setAnchorPoint(Vec2(0,0.5));
    line->setPositionZ(-1);
    Board::Instance->game->addChild(line);
}

void User::update(float dt){
    updatePhysics(dt);
    if(abs(velocity.x) < 0.1 && abs(velocity.y) < 25 || position.y < -200)
        Reset();
    userPosition = position;
    Board::PrintVec2("UserPosition", GetBounds().origin);
}

void User::updatePhysics(float dt){
    if(!isHooked){
        acceleration.x = FALL_SPEED * Board::Instance->gravity.x * dt;
        acceleration.y = FALL_SPEED * Board::Instance->gravity.y * dt;
        velocity.x += acceleration.x;
        velocity.y += acceleration.y;
        Vec2 tempPos;
        tempPos.x = position.x + velocity.x * dt;
        tempPos.y = position.y + velocity.y * dt;
        SetPosition(tempPos);
    } else {
        hookArmAngle = fmod(hookArmAngle, 360);
        float angularAccelerationFromGravity = cos(hookArmAngle / 57.29) * (Board::Instance->gravity.y * FALL_SPEED * 0.7);
        angularVelocity -= (angularAccelerationFromGravity * dt);
        hookArmAngle += angularVelocity * dt;
        SetPositionToArmAngle();
    }
}

void User::Snag(){
    closest = Board::Instance->GetClosestHook(position);
    Vec2 closestPos = closest->GetPosition();
    hookArmDistance = closest->GetPosition().distance(position);
    std::cout << "Distance: " << std::to_string(hookArmDistance);
    hookArmAngle = atan((closestPos.y - position.y)/(position.x - closestPos.x)) * 57.29;
    closestPosition = closest->GetPosition();
    if(position.x - closestPos.x > 0) hookArmAngle += 360;
    else hookArmAngle += 180;
    
    if(hookArmDistance < 500 && hookArmDistance > 10){
        std::cout << " Angle: " << std::to_string(hookArmAngle);
        FindAngularVelocity();
        isHooked = true;
        line->setVisible(true);
        lineBaseScale = hookArmDistance / line->getTexture()->getPixelsWide();
    }
}

void User::FindAngularVelocity(){
    Vec2 tempVelocity((0.5*sin(hookArmAngle / 57.29) + 0.5) * velocity.x, (0.5 * cos(hookArmAngle / 57.29) + 0.5) * velocity.y);
    float tempTotal = sqrt(pow(tempVelocity.x,2) + pow(tempVelocity.y, 2));
    angularVelocity = (tempTotal / hookArmDistance) * (tempVelocity.x / abs(tempVelocity.x));
    velocity = tempVelocity;
}

void User::SetPositionToArmAngle(){
    float tempX = (cos(hookArmAngle / 57.29) * hookArmDistance) + closestPosition.x;
    float tempY = (-1 * (sin(hookArmAngle / 57.29) * hookArmDistance)) + closestPosition.y;
    SetPosition(Vec2(tempX,tempY));
}

void User::Release(){
    if(isHooked == true){
        isHooked = false;
        float totalVelocity = abs(hookArmDistance * angularVelocity) / 50;
        int sign = 0;
        if(angularVelocity > 0)
            sign = 1;
        if(angularVelocity == 0)
            sign = 0;
        if(angularVelocity < 0)
            sign = -1;
        float releaseAngle = hookArmAngle + (sign * 90);
        velocity.x = cos(releaseAngle / 57.29) * totalVelocity;
        velocity.y = -1 * sin(releaseAngle / 57.29) * totalVelocity;
        velocity *= 1.15;
        line->setVisible(false);
    }
}

void User::collide(Vec2 side){
    if(isHooked)
        Release();
    else{
        if(side.x !=0){
            velocity.x *= -0.9;
            velocity.y *= 0.9;
        }
        if(side.y !=0){
            velocity.y *= -0.9;
            velocity.x *= 0.9;
        }
    }
}

void User::Reset(){
    position.set(Spawner::Instance->GetPosition());
    acceleration.x = 0;
    acceleration.y = 0;
    velocity = Spawner::Instance->GetVelocity();
}

void User::CalculateScale(){
    float distanceToGround = 0 - position.y; // board.ground;
    float newDistanceToGround = distanceToGround / boardScale;
    sprite->setPosition((Board::Instance->visibleSize.width / 2.0), 0 -newDistanceToGround);
    //Why is this scale here?
    SetScale(Vec2((1/boardScale),(1/boardScale)));
    if(isHooked){
        line->setPosition(closest->GetSprite()->getPosition());
        line->setScale(lineBaseScale*(1/boardScale), 10.0 * (1/boardScale));
        line->setRotation(hookArmAngle);
    }
}