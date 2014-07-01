//
//  Entity.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#include "Entity.h"
#include "Board.h"

USING_NS_CC;

#define DEBUG_PHYSICS false

Vec2 Entity::baseScale;
float Entity::boardScale;
Vec2 Entity::userPosition;

Entity::Entity(char* texture,int x, int y){
    sprite = Sprite::create(texture);
    sprite->setPosition(Point(x,y));
    Board::Instance->AddEntity(this);
    this->position.set(x,y);
    spriteBaseScale.set(1.0,1.0);
    imageSize = Vec2(sprite->getTexture()->getPixelsHigh(),sprite->getTexture()->getPixelsWide());
    auto tempDebug = DrawNode::create();
    boundsDebug = tempDebug;
    Vec2 vertices[] = {Vec2(0,0),
                       Vec2(0,imageSize.y),
                       Vec2(imageSize.x, imageSize.y),
                       Vec2(imageSize.x, 0)};
    //boundsDebug->drawPolygon(vertices, 4, Color4F(0.0f,0.0f,1.0f,0.5f), 0, Color4F(0.2f,0.2f,0.2f,0.0f));
    //Board::Instance->game->addChild(boundsDebug);
    //boundsDebug->setPosition(GetBounds().origin.x,GetBounds().origin.y);
}

Sprite* Entity::GetSprite(){
    return sprite;
}

void Entity::SetPosition(Point point){
    Point goodPoint = Board::Instance->TryMove(point,GetBounds());
    if(goodPoint.y != 999999999)
        this->position = goodPoint;
}

Point Entity::GetPosition(){
    return position;
}

void Entity::update(float dt){
    //boundsDebug->setPosition(GetBounds().origin.x,GetBounds().origin.y);
    //UpdateSprite();
}

void Entity::SetScale(Vec2 scale){
    sprite->setScale(scale.x * spriteBaseScale.x, scale.y * spriteBaseScale.y);
}

Vec2 Entity::GetScale(){
    return scale;
}

void Entity::SetBaseScale(Vec2 scale){
    spriteBaseScale = scale;
    //boundsDebug->setScale(spriteBaseScale.x, spriteBaseScale.y);
}

Rect Entity::GetBounds(){
    Rect myBounds = sprite->getBoundingBox();
    myBounds.origin = Point(position.x - (imageSize.x * spriteBaseScale.x)/ 2.0,position.y - (imageSize.y * spriteBaseScale.y) / 2.0);
    myBounds.size = Size(imageSize.x * spriteBaseScale.x, imageSize.y * spriteBaseScale.y);
    return myBounds;
}

void Entity::UpdateSprite(){
    this->CalculateScale();
}

void Entity::CalculateScale(){
    //Board::PrintVec2("userPosition",userPosition);
    float distanceToCenter = position.x - userPosition.x;
    float newDistanceToCenter = distanceToCenter / boardScale;
    float distanceToGround = 0 - position.y; // board.ground;
    float newDistanceToGround = distanceToGround / boardScale;
    sprite->setPosition((Board::Instance->visibleSize.width / 2.0) + newDistanceToCenter, 0 - newDistanceToGround);
    SetScale(Vec2((1/boardScale),(1/boardScale)));
}
