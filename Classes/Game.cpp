#include <stdio.h>
#include "Game.h"
#include "PlayScreen.h"
#include "Entity.h"


void Game::update(float dt){
    user->update();
    if(user->getPosition().y < -400)
        reset(currentLevel);
    updateGravity();
    updateScale();
    updateFocusPoint();
}

Entity* Game::getClosestHook(Point pos){
    float smallestDistance = 0; //Gets set no matter what if closets is NULL
    Entity* closestHook = NULL;
    std::vector<Entity*> ents = currentLevel->getEntities();
    for(int x = 0; x < ents.size(); x++){
        if(TAG_TO_TYPE(ents[x]->getTag()) == HOOK){
            float distance = ents[x]->getPosition().getDistance(pos);
            if(distance <= smallestDistance || closestHook == NULL){
                smallestDistance = distance;
                closestHook = (Entity*)ents[x];
            }
        }
    }
    return closestHook;
}

bool Game::onContact(PhysicsContact& contact){
    int tag = contact.getShapeB()->getBody()->getTag();
    int type = TAG_TO_TYPE(tag);
    switch(type) {
        case SPIKE_WALL:
            user->Reset();
            return false;
            break;
        case GOAL:
            PlayScreen::Instance->onWin();
            return false;
            break;
        case WALL:
            closing = false; //ShiftLow();
            user->Bounce(contact.getContactData());
            break;
    }
    return true;
}

void Game::release(){
    user->Release();
    shiftLow();
}

bool Game::snag(){
    if(!user->isHooked()){
        user->Snag();
        shiftLow();
        return true;
    }
    return false;
}

void Game::reset(){
    user->reset(currentLevel->getLaunchPosition(), currentLevel->getLaunchVelocity());
    focusPoint = Vec2(currentLevel->getLaunchVelocity().x, currentLevel->getLaunchVelocity().y);
    previousTargetChange = Vec2(0,0);
    catching = false;
    closing = false;
    startClosingDistance = 0;
    startClosingMAX_SHIFT = 30;
    previousTrail = 0;
    MAX_SHIFT = 30;
    PlayScreen::Instance->resetTime();
}

void Game::loadLevel(Level* lvl){
    if(currentLevel != NULL)
        clear();
    currentLevel = lvl;
    currentLevel->add();
    PlayScreen::Instance->initWinPopUp();
}

void Game::clear(){
    currentLevel->remove();
}

void Game::shiftLow(){
    MAX_CHANGE = 9;
}

void Game::updateGravity(){
    if(user->isHooked()){
        if(user->getVelocity().y > 0)
            gravity = (Vec2(0,-240));
        else
            gravity = (Vec2(0,-270));
    }
    if(!user->isHooked())
        gravity = (Vec2(0,-180));
}

void Game::updateScale(){
    Vec2 visibleSize = PlayScreen::Instance->getVisibleSize();
    scale = visibleSize.y / (user->getPosition().y * 2 + (visibleSize.y / 3));
    over = 0;
    scale = fmod(scale, 1.0);
    if(scale < 0.5){
        scale = 0.5;
        over = (user->getPosition().y * 2 + visibleSize.y / 3) - 2.0 * (visibleSize.y);
    }
}

void Game::updateFocusPoint(){
    Vec2 targetFocusPoint = getTargetFocusPoint();
    
    if(focusPoint.getDistance(targetFocusPoint) < 15.f + MAX_CHANGE){
        previousTargetChange = targetFocusPoint - focusPoint;
        focusPoint = targetFocusPoint;
        MAX_CHANGE = 20;
        return;
    } else {
        if(focusPoint.getDistance(targetFocusPoint) >75.f)
            MAX_CHANGE += 0.4;
    }
    Vec2 angleTemp1(focusPoint.x - user->getPosition().x, focusPoint.y - user->getPosition().y);
    Vec2 angleTemp2(targetFocusPoint.x - user->getPosition().x, targetFocusPoint.y - user->getPosition().y);
    float anglePercent = 1.f - (fmod(getAngle(angleTemp1, angleTemp2), 180.f) / 180.f);
    Vec2 visibleSize = PlayScreen::Instance->getVisibleSize();
    float MAX_COMP_CHANGE = MAX_CHANGE * anglePercent * ((visibleSize.x * visibleSize.y)/(1704.f*960.f));
    Vec2 targetChange = targetFocusPoint - focusPoint;
    Vec2 targetVelChange = targetChange - previousTargetChange;
    float xRatio = fabs(MAX_COMP_CHANGE / targetVelChange.x);
    float yRatio = fabs(MAX_COMP_CHANGE / targetVelChange.y);
    float minRatio = 1;
    if(xRatio < yRatio)
        minRatio = xRatio;
    else
        minRatio = yRatio;
    if(minRatio < 1.0){
        targetVelChange.x *= minRatio;
        targetVelChange.y *= minRatio;
    }
    Vec2 finalChange = Vec2(targetVelChange.x + previousTargetChange.x * (1-anglePercent), targetVelChange.y + previousTargetChange.y * (1-anglePercent));
    previousTargetChange = finalChange;
    focusPoint = Vec2(focusPoint.x + finalChange.x, focusPoint.y + finalChange.y);
}

Vec2 Game::getTargetFocusPoint(){
    Vec2 targetFocusPoint = Vec2(user->getPosition().x, user->getPosition().y);
    
    if(user->isHooked())
        return GetTween(targetFocusPoint, user->GetClosestPosition(),0.5f);
    
    closing = false;
    float velocityPercent = 0.5 * 1.25;
    Vec2 velocityOffset = Vec2(user->getVelocity().x * velocityPercent, user->getVelocity().y * velocityPercent);
    targetFocusPoint += velocityOffset;
    
    Vec2 visibleSize = PlayScreen::Instance->getVisibleSize();
    
    Vec2 userCenter = Vec2(user->getPosition().x + (1 * ((visibleSize.x /2) - ((focusPoint.x * 1)))), user->getPosition().y);
    Vec2 velocityPoint = userCenter + velocityOffset;
    Vec2 targetPointVector = velocityPoint - userCenter;
    targetPointVector.normalize();
    
    Vec2 startPos = userCenter + targetPointVector * (user->getSize().x / 2.0) * 1.1;
    Vec2 contact = velocityPoint;
    
    PhysicsShape* body;
    Entity* contactEnt = NULL;
    bool bouncingVertical = true;
    
    auto func = [&contact, &body](PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data)->bool{
        contact = info.contact;
        body = info.shape;
        return true;
    };
    PlayScreen::Instance->raycast(func, startPos, velocityPoint, nullptr);
    
    if(TAG_TO_TYPE(body->getTag()) == WALL)
        contactEnt = currentLevel->getEntity(TAG_TO_ID(body->getTag()));
    
    startPos.x -= (1 * ((visibleSize.x /2) - ((focusPoint.x * 1))));
    velocityPoint.x -= (1 * ((visibleSize.x /2) - ((focusPoint.x * 1))));
    contact.x -= (1 * ((visibleSize.x /2) - ((focusPoint.x * 1))));
    
    //_drawnode->drawSegment(startPos, velocityPoint, 1, Color4F::RED);
    //_drawnode->drawDot(contact, 9, Color4F(1.0f, 1.0f, 1.0f, 1.0f));
    
    if(contactEnt != NULL){
        Rect bBox = contactEnt->getBoundingBox();
        if(fabs(contact.x - bBox.getMinX()) < 1 || fabs(contact.x - bBox.getMaxX()) < 1)
            bouncingVertical = false;
        if(bouncingVertical)
            targetFocusPoint.y = contact.y;
        else
            targetFocusPoint.x = contact.x;
    }
    return targetFocusPoint;
}

Vec2 Game::getGravity(){
    return gravity;
};
Vec2 Game::getFocusPoint(){
    return focusPoint;
};
float Game::getScale(){
    return scale;
};