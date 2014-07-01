//
//  Wall.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/29/14.
//
//

#include "Wall.h"

int Wall::type;

Wall::Wall(Vec2 position, Vec2 size) : Entity("wall.png",position.x + size.x/2.0, position.y - size.y/2.0){
    SetBaseScale(Vec2((size.x) / imageSize.x, (size.y) / imageSize.y));
    SetUpPhysicsSprite("wall.png");
    type = 0;
    body->setTag(0);
}

void Wall::update(float dt){
    Entity::update(dt);
}