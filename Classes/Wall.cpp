//
//  Wall.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/29/14.
//
//

#include "Wall.h"

Wall::Wall(Vec2 position, Vec2 size) : Entity("wall.png",position.x + size.x/2.0, position.y - size.y/2.0){
    SetBaseScale(Vec2((size.x) / imageSize.x, (size.y) / imageSize.y));
}

void Wall::update(float dt){
    Entity::update(dt);
}