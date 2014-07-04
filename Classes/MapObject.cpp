//
//  MapObject.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#include "MapObject.h"
#include "LevelEditor.h"
#include "MainMenu.h"

Vec2 MapObject::origin;

MapObject::MapObject(Vec2 start, int type){
    this->type = type;
    this->startCoord = start;
    this->endCoord = Vec2(start.x + 1, start.y + 1);
    switch(type){
        case WALL:
            sprite = Sprite::create("wall.png");
            sprite->setScale((float)(abs(endCoord.x - startCoord.x) * TILE_SIZE) / sprite->getTexture()->getPixelsWide(), (float)(abs(endCoord.y - startCoord.y) * TILE_SIZE) / sprite->getTexture()->getPixelsHigh());
            break;
        case SPIKE_WALL:
            sprite = Sprite::create("spikewall.png");
            sprite->setScale((float)(abs(endCoord.x - startCoord.x) * TILE_SIZE) / sprite->getTexture()->getPixelsWide(), (float)(abs(endCoord.y - startCoord.y) * TILE_SIZE) / sprite->getTexture()->getPixelsHigh());
            break;
        case SPAWNER:
            sprite = Sprite::create("spawner.png");
            LevelEditor::Instance->DisableSpawner();
            break;
        case HOOK:
            sprite = Sprite::create("hook.png");
            break;
        case NO_TOOL:
            break;
        case ERASE:
            break;
    }
    sprite->setPosition(startCoord.x * TILE_SIZE, startCoord.y * TILE_SIZE);
    sprite->setAnchorPoint(Point(0,0));
    sprite->setPositionZ(-3);
    SetOriginTile(origin);
    
}
void MapObject::UpdateEndCoord(Vec2 coord){
    this->endCoord = coord;
    if(endCoord.x < startCoord.x && endCoord.y < startCoord.y){
        sprite->setAnchorPoint(Point(0,0));
        sprite->setPosition(endCoord.x * TILE_SIZE, endCoord.y * TILE_SIZE);
    }
    switch(type){
        case WALL:
            sprite->setScale((float)(abs(endCoord.x - startCoord.x) * TILE_SIZE) / sprite->getTexture()->getPixelsWide(), (float)(abs(endCoord.y - startCoord.y) * TILE_SIZE) / sprite->getTexture()->getPixelsHigh());
            break;
        case SPIKE_WALL:
            sprite->setScale((float)(abs(endCoord.x - startCoord.x) * TILE_SIZE) / sprite->getTexture()->getPixelsWide(), (float)(abs(endCoord.y - startCoord.y) * TILE_SIZE) / sprite->getTexture()->getPixelsHigh());
            break;
        case SPAWNER:
            break;
        case HOOK:
            break;
        case NO_TOOL:
            break;
        case ERASE:
            break;
    }
}
Sprite* MapObject::GetSprite(){
    return sprite;
}

void MapObject::SetOriginTile(Vec2 tile){
    origin = tile;
    sprite->setPosition((startCoord.x - tile.x) * TILE_SIZE, (startCoord.y - tile.y) * TILE_SIZE);
}

int MapObject::GetType(){
    return type;
}

Vec2 MapObject::GetStart(){
    return Vec2(startCoord.x * (MainMenu::screenScale.x * 256 * 0.5), startCoord.y * (MainMenu::screenScale.x * 256 * 0.5) + GetSize().y);
}

Vec2 MapObject::GetSize(){
    return Vec2((endCoord.x - startCoord.x) * (MainMenu::screenScale.x * 256 * 0.5), (endCoord.y - startCoord.y) * (MainMenu::screenScale.y * 256 * 0.5));
}
