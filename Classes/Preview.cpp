//
//  Preview.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 8/13/14.
//
//

#include "Preview.h"
#include "cocos2d.h"

Preview::Preview(Rect viewport, Layer* layer, float scale){
    this->layer = layer;
    screenViewOrigin = viewport.origin;
    screenViewSize = viewport.size;
    mapViewOrigin = Vec2(0,0);
    mapViewScale = scale;
    mapViewSize = Vec2(screenViewSize.x/mapViewScale, screenViewSize.y/mapViewScale);
};
void Preview::Update(){
    
}
void Preview::AddEntity(Entity* ent){
    std::string textures[6] = {"wall.png", "spikewall.png", "hook.png", "spawner.png", "goal.png", "user.png"};
    float baseScales[6] = {1,1,1.5,1,1,0.5};
    entities[ent->ID] = ent;
    sprites[ent->ID] = Sprite::create(textures[ent->GetType()]);
    sprites[ent->ID]->setPosition(MapToScreen(ent->GetPosition()));
    sprites[ent->ID]->setAnchorPoint(Vec2(0,0));
    if(ent->GetType() != SPAWNER && ent->GetType() != HOOK)
        sprites[ent->ID]->setScale((ent->GetSize().x * mapViewScale)/sprites[ent->ID]->getBoundingBox().size.width, (ent->GetSize().y * mapViewScale) / sprites[ent->ID]->getBoundingBox().size.height);
    else
            sprites[ent->ID]->setScale(baseScales[ent->GetType()] * mapViewScale,baseScales[ent->GetType()] * mapViewScale);
    layer->addChild(sprites[ent->ID]);
};
void Preview::RemoveEntity(Entity* ent){
    layer->removeChild(sprites[ent->ID]);
    sprites.erase(ent->ID);
    entities.erase(ent->ID);
};
void Preview::RemoveEntity(int entID){
    layer->removeChild(sprites[entID]);
    sprites.erase(entID);
    entities.erase(entID);
};
Entity* Preview::PopEntity(Entity* ent){
    layer->removeChild(sprites[ent->ID]);
    sprites.erase(ent->ID);
    Entity* returnEnt = entities[ent->ID];
    entities.erase(ent->ID);
    return returnEnt;
};
Entity* Preview::PopEntity(int entID){
    layer->removeChild(sprites[entID]);
    sprites.erase(entID);
    Entity* returnEnt = entities[entID];
    entities.erase(entID);
    return returnEnt;
};
void Preview::SetZoom(float newScale){
    mapViewScale = newScale;
    mapViewSize = Vec2(screenViewSize.x/mapViewScale, screenViewSize.y/mapViewScale);
    Update();
};
void Preview::SetDrag(float){

};
Entity* Preview::CreateEntity(Vec2 startTouch, Vec2 endTouch, int type){
    Vec2 minCoord;
    Vec2 maxCoord;
    if(startTouch.x > endTouch.x){
        maxCoord.x = startTouch.x;
        minCoord.x = endTouch.x;
    } else{
        maxCoord.x = endTouch.x;
        minCoord.x = startTouch.x;
    }
    if(startTouch.y > endTouch.y){
        maxCoord.y = startTouch.y;
        minCoord.y = endTouch.y;
    } else {
        maxCoord.y = endTouch.y;
        minCoord.y = startTouch.y;
    }
    Vec2 size(maxCoord.x - minCoord.x, maxCoord.y - minCoord.y);
    Vec2 pos = minCoord;
    Entity* ent;
    switch(type){
        case WALL:
            ent = Entity::createWall(pos,size);
            break;
        case SPIKE_WALL:
            ent = Entity::createSpikeWall(pos,size);
            break;
        case GOAL:
            ent = Entity::createGoal(pos,size);
        default:
            ent = new Entity(pos, size, Vec2(0,0), type);
    }
    return ent;
};
Entity* Preview::GetTarget(Vec2 touch){

};
Vec2 Preview::ScreenToMap(Vec2 pos){
    Vec2 posPastScreenOrigin(pos.x - screenViewOrigin.x, pos.y - screenViewOrigin.y);
    Vec2 viewPercent(posPastScreenOrigin.x / screenViewSize.x, posPastScreenOrigin.y / screenViewSize.y);
    Vec2 mapPosWithoutOrigin(viewPercent.x/mapViewScale, viewPercent.y/mapViewScale);
    Vec2 mapPos(mapPosWithoutOrigin.x + mapViewOrigin.x, mapPosWithoutOrigin.y + mapViewOrigin.y);
    return mapPos;
}
Vec2 Preview::MapToScreen(Vec2 pos){
    Vec2 mapPosWithoutOrigin(pos.x - mapViewOrigin.x, pos.y - mapViewOrigin.y);
    Vec2 viewPercent(mapPosWithoutOrigin.x*mapViewScale, mapPosWithoutOrigin.y*mapViewScale);
    Vec2 screenPos(viewPercent.x + screenViewOrigin.x, viewPercent.y + screenViewOrigin.y);
    return screenPos;
}