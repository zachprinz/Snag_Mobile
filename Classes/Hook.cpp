//
//  Hook.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "Hook.h"
#include "Board.h"

Hook::Hook(Vec2 position) : Entity("hook.png", position.x,position.y){
    this->position = position;
    SetBaseScale(Vec2(1.5,1.5));
}
void Hook::update(float dt){
    Entity::update(dt);
    Board::PrintVec2("HookPosition", GetBounds().origin);
}