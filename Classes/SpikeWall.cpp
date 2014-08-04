//
//  SpikeWall.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/30/14.
//
//

#include "SpikeWall.h"

int SpikeWall::type;

SpikeWall::SpikeWall(Vec2 position, Vec2 size) : Entity("spikewall.png",position.x + size.x/2.0, position.y - size.y/2.0, 1){
    SetBaseScale(Vec2((size.x) / imageSize.x, (size.y) / imageSize.y));
    SetUpPhysicsSprite("spikewall.png");
    type = 1;
    body->setTag(1);
    body->setContactTestBitmask(true);
    body->setCategoryBitmask(true);
    //body->setCollisionBitmask(true);

}