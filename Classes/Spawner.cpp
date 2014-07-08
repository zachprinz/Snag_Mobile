//
//  Spawner.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/30/14.
//
//

#include "Spawner.h"
#include "Board.h"

Spawner* Spawner::Instance;

Spawner::Spawner(Vec2 pos, Vec2 vel) : Entity("spawner.png", pos.x, pos.y, 3){
    velocity = vel;
    Instance = this;
    //Board::Instance->user->Reset();
    SetUpPhysicsSprite("spawner.png");
}

Vec2 Spawner::GetVelocity(){
    return velocity;
}


