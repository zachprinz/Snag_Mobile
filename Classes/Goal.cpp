//
//  Goal.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/6/14.
//
//

#include "Goal.h"

int Goal::type;

Goal::Goal(Vec2 position, Vec2 size) : Entity("goal.png",position.x + size.x/2.0, position.y - size.y/2.0, 4){
    SetBaseScale(Vec2((size.x) / imageSize.x, (size.y) / imageSize.y));
    SetUpPhysicsSprite("goal.png");
    type = -1;
    body->setTag(-1);
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
    body->setCollisionBitmask(true);
    
}

void Goal::update(float dt){
    Entity::update(dt);
}