//
//  Preview.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 8/13/14.
//
//

#include "Preview.h"
#include "MainMenu.h"
#include "cocos2d.h"

Preview::Preview(Rect viewport, Layer* layer, float scale){
    this->layer = layer;
    screenViewOrigin = viewport.origin;
    screenViewSize = viewport.size;
    mapViewOrigin = Vec2(0,0);
    mapViewScale = scale;
    mapViewScaleOriginal = scale;
    originalMapViewScale = scale;
    
    stencil = DrawNode::create();
    Vec2 rectangle[4];
    //rectangle[0] = Vec2(0,0);//screenViewOrigin;
    //rectangle[1] = Vec2(0,screenViewSize.y);//Vec2(screenViewOrigin.x, screenViewOrigin.y + screenViewSize.y);
    //rectangle[2] = screenViewSize;//Vec2(screenViewOrigin.x + screenViewOrigin.x, screenViewOrigin.y + screenViewSize.y);
    //rectangle[3] = Vec2(screenViewSize.x, 0);//Vec2(screenViewOrigin.x, screenViewOrigin.y + screenViewSize.y);
    
    rectangle[0] = screenViewOrigin;
    rectangle[1] = Vec2(screenViewOrigin.x, screenViewOrigin.y + screenViewSize.y);
    rectangle[2] = Vec2(screenViewOrigin.x + screenViewOrigin.x, screenViewOrigin.y + screenViewSize.y);
    rectangle[3] = Vec2(screenViewOrigin.x, screenViewOrigin.y + screenViewSize.y);
    
    Color4F white(1, 1, 1, 1);
    stencil->drawPolygon(rectangle, 4, white, 1, white);
    //stencil->setPosition(Vec2(0,0));
    stencil->setAnchorPoint(Vec2(0,0));
    //stencil->setContentSize(Size(MainMenu::screenSize.x, MainMenu::screenSize.y));//screenViewSize.x, screenViewSize.y));
    
    clipNode = ClippingNode::create();
    clipNode->setAlphaThreshold(0);
    clipNode->setAnchorPoint(Vec2(0,0));
    clipNode->setGlobalZOrder(1);
    layer->addChild(clipNode,1);
    layer->addChild(stencil);
    clipNode->setStencil(stencil);
};
void Preview::Update(){
    for (std::map<int,Entity*>::iterator it=entities.begin(); it!=entities.end(); ++it){
        int entID = ((Entity*)it->second)->ID;
        UpdateSprite(entID);
    }
}
void Preview::UpdateSprite(int ID){
    float baseScales[6] = {1,1,1.5,1,1,0.5};
    sprites[ID]->setPosition(MapToScreen(entities[ID]->GetPosition()));
    if(entities[ID]->GetType() != SPAWNER && entities[ID]->GetType() != HOOK)
        sprites[ID]->setScale((entities[ID]->GetSize().x * mapViewScale)/entities[ID]->GetOriginalSize().x, (entities[ID]->GetSize().y * mapViewScale) / entities[ID]->GetOriginalSize().y);
    else
        sprites[ID]->setScale(baseScales[entities[ID]->GetType()] * mapViewScale,baseScales[entities[ID]->GetType()] * mapViewScale);
}
void Preview::AddEntity(Entity* ent){
    std::string textures[6] = {"wall.png", "spikewall.png", "hook.png", "spawner.png", "goal.png", "user.png"};
    float baseScales[6] = {1,1,1.5,1,1,0.5};
    entities[ent->ID] = ent;
    sprites[ent->ID] = Sprite::create(textures[ent->GetType()]);
    sprites[ent->ID]->setPosition(MapToScreen(ent->GetPosition()));
    sprites[ent->ID]->setAnchorPoint(Vec2(0.5,0.5));
    if(ent->GetType() != SPAWNER && ent->GetType() != HOOK)
        sprites[ent->ID]->setScale((ent->GetSize().x * mapViewScale)/sprites[ent->ID]->getBoundingBox().size.width, (ent->GetSize().y * mapViewScale) / sprites[ent->ID]->getBoundingBox().size.height);
    else
            sprites[ent->ID]->setScale(baseScales[ent->GetType()] * mapViewScale,baseScales[ent->GetType()] * mapViewScale);
    clipNode->addChild(sprites[ent->ID]);
};
void Preview::RemoveEntity(Entity* ent){
    clipNode->removeChild(sprites[ent->ID]);
    sprites.erase(ent->ID);
    entities.erase(ent->ID);
};
void Preview::RemoveEntity(int entID){
    clipNode->removeChild(sprites[entID]);
    sprites.erase(entID);
    entities.erase(entID);
};
Entity* Preview::PopEntity(Entity* ent){
    clipNode->removeChild(sprites[ent->ID]);
    sprites.erase(ent->ID);
    Entity* returnEnt = entities[ent->ID];
    entities.erase(ent->ID);
    return returnEnt;
};
Entity* Preview::PopEntity(int entID){
    clipNode->removeChild(sprites[entID]);
    sprites.erase(entID);
    Entity* returnEnt = entities[entID];
    entities.erase(entID);
    return returnEnt;
};
void Preview::SetZoom(float newScale){
    if(newScale < 0.5)
        newScale = 0.5;
    if(newScale > 2.0)
        newScale = 2.0;
    mapViewScale = mapViewScaleOriginal * newScale;
    printf("\nUpdating for Zoom");
    Update();
};
void Preview::onTouchesBegan(std::vector<Touch*> touches){
    Vec2 touchOne = touches[0]->getLocation();
    Vec2 touchTwo = touches[1]->getLocation();
    originalMultitouchDistance = touchOne.getDistance(touchTwo);
};
void Preview::onTouchesMoved(std::vector<Touch*> touches){
    if(originalMultitouchDistance == 0){
        onTouchesBegan(touches);
    } else {
        Vec2 touchOne = touches[0]->getLocation();
        Vec2 touchTwo = touches[1]->getLocation();
        currentMultitouchDistance = touchOne.getDistance(touchTwo);
        float ratio = currentMultitouchDistance / originalMultitouchDistance;
        SetZoom(ratio);
    }
};
void Preview::onTouchesEnded(std::vector<Touch*> touches){
   mapViewScaleOriginal = mapViewScale;
    originalMultitouchDistance = 0;
}
void Preview::Drag(Vec2 difference){
    mapViewOrigin += Vec2(difference.x/mapViewScale, difference.y/mapViewScale);
    printf("\nUpdating for Drag");
    Update();
};
Entity* Preview::CreateEntity(Vec2 startTouch2, Vec2 endTouch2, int type){
    Vec2 startTouch = ScreenToMap(startTouch2);
    printf("\nScreen Start: (%f, %f)    Map Start: (%f, %f)", startTouch2.x,startTouch2.y,startTouch.x,startTouch.y);
    Vec2 endTouch = ScreenToMap(endTouch2);
    printf("\nScreen End:   (%f, %f)    Map End:   (%f, %f)", endTouch2.x, endTouch2.y, endTouch.x,endTouch.y);
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
    pos = Vec2(pos.x + (size.x / 2.0), pos.y + (size.y / 2.0));
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
    for (std::map<int,Sprite*>::iterator it=sprites.begin(); it!=sprites.end(); ++it){
        if(it->second->getBoundingBox().containsPoint(touch)){
            return entities[it->first];
        }
    }
};
Vec2 Preview::ScreenToMap(Vec2 pos){
    Vec2 posOnViewport(pos.x - screenViewOrigin.x, pos.y - screenViewOrigin.y);
    Vec2 posMapWithoutOrigin(posOnViewport.x / mapViewScale, posOnViewport.y / mapViewScale);
    Vec2 mapPos(posMapWithoutOrigin.x + mapViewOrigin.x, posMapWithoutOrigin.y + mapViewOrigin.y);
    return mapPos;
}
Vec2 Preview::MapToScreen(Vec2 pos){
    Vec2 mapPosWithoutOrigin(pos.x - mapViewOrigin.x, pos.y - mapViewOrigin.y);
    Vec2 viewPercent(mapPosWithoutOrigin.x*mapViewScale, mapPosWithoutOrigin.y*mapViewScale);
    Vec2 screenPos(viewPercent.x + screenViewOrigin.x, viewPercent.y + screenViewOrigin.y);
    return screenPos;
}
float Preview::GetScale(){
    return mapViewScale;
}
void Preview::Reset(){
    mapViewOrigin = Vec2(0,0);
    mapViewScale = originalMapViewScale;
}