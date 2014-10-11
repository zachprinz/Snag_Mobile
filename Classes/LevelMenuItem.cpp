//
//  LevelMenuItem.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/3/14.
//
//

#include "LevelMenuItem.h"
#include "LevelSelect.h"
#include "MainMenu.h"
#include "Level.h"

int LevelMenuItem::count;

LevelMenuItem::LevelMenuItem(int position, ui::ScrollView* layer, float center, float inlayHeight){
    float yPositions[] = {0.24,0.39,0.39 + (.37-.22),0.39 + (2*(0.37-.22))};
    float xPosition = 0.0125;
    float x = xPosition;
    float y = yPositions[count];
    count++;
    Vector<MenuItem*> menuItems;
    std::string levelImageName = "Level_Left.png";
    if((position+1) % 2 == 0)
        levelImageName = "Level_Right.png";
    background = MainMenu::CreateButton("levelselect", levelImageName);
    background->setGlobalZOrder(0);
    background->setScaleX(background->getScaleX() * 0.98);
    background->setPosition(Vec2(center,(inlayHeight) - floor(((int)position)/2)*background->getBoundingBox().size.height*1.1));
    background->setAnchorPoint(Vec2((position+1) % 2 - (0.075*(0.5-((position+1)%2))),1.0));
    menuItems.pushBack(background);
    
    favorited = MainMenu::CreateButton("FavoriteOff.png", LevelSelect::Instance, menu_selector(LevelSelect::favoriteCallback), Vec2(x+0.00, 1.0-(y+0.00)), Vec2(1,0));
    favorited->setPosition(Vec2(background->getBoundingBox().getMaxX() - (24*MainMenu::screenScale.y), background->getBoundingBox().getMidY()));
    favorited->setAnchorPoint(Vec2(1,0.5));
    favorited->setScale(favorited->getScaleX() * 0.925, favorited->getScaleY() * 0.925);
    favorited->setGlobalZOrder(0);
    //menuItems.pushBack(favorited);
    
    this->favorites = MainMenu::CreateLabel("0", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->favorites->setPosition(Vec2(favorited->getBoundingBox().getMinX() - (30 * MainMenu::minScreenScale), favorited->getBoundingBox().getMidY()));
    this->favorites->setAnchorPoint(Vec2(1,0.5));
    this->favorites->setGlobalZOrder(0);
    
    this->number = MainMenu::CreateLabel("1) ", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->number->setPosition(Vec2(background->getPositionX() + (24*MainMenu::screenScale.x), background->getBoundingBox().getMidY()));
    this->number->setAnchorPoint(Vec2(0,0.5));
    this->number->setGlobalZOrder(0);
    
    this->name = MainMenu::CreateLabel("New Level", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->name->setPosition(Vec2(number->getBoundingBox().getMaxX(), background->getBoundingBox().getMidY()));
    this->name->setAnchorPoint(Vec2(0,0.5));
    this->name->setGlobalZOrder(0);
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,0));
    menu->setPosition(0,0);
    
    layer->addChild(menu,-11);
    //layer->addChild(name,-11);
    //layer->addChild(number,-11);
    //layer->addChild(favorites,-11);
}

LevelMenuItem::LevelMenuItem(int position, Layer* layer, float center, float inlayHeight){
    float yPositions[] = {0.24,0.39,0.39 + (.37-.22),0.39 + (2*(0.37-.22))};
    float x = center;
    float y = yPositions[count];
    count++;
    Vector<MenuItem*> menuItems;
    
    background = MainMenu::CreateButton("LSSingleLevelBackground.png", LevelSelect::Instance, menu_selector(LevelSelect::selectCallback), Vec2(x,1.0-y), Vec2(0,0));
    background->setGlobalZOrder(0);
    background->setScaleX(background->getScaleX() * 0.98);
    background->setPositionX(x);
    background->setPositionY(2100);
    background->setAnchorPoint(Vec2(0.5,0.5));
    menuItems.pushBack(background);
    
    favorited = MainMenu::CreateButton("FavoriteOff.png", LevelSelect::Instance, menu_selector(LevelSelect::favoriteCallback), Vec2(x+0.00, 1.0-(y+0.00)), Vec2(1,0));
    favorited->setPosition(Vec2(background->getBoundingBox().getMaxX() - (24*MainMenu::screenScale.y), background->getBoundingBox().getMidY()));
    favorited->setAnchorPoint(Vec2(1,0.5));
    favorited->setScale(favorited->getScaleX() * 0.925, favorited->getScaleY() * 0.925);
    favorited->setGlobalZOrder(0);
    menuItems.pushBack(favorited);
    
    this->favorites = MainMenu::CreateLabel("0", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->favorites->setPosition(Vec2(favorited->getBoundingBox().getMinX() - (30 * MainMenu::minScreenScale), favorited->getBoundingBox().getMidY()));
    this->favorites->setAnchorPoint(Vec2(1,0.5));
    this->favorites->setGlobalZOrder(0);
    
    this->number = MainMenu::CreateLabel("1) ", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->number->setPosition(Vec2(background->getPositionX() + (24*MainMenu::screenScale.x), background->getBoundingBox().getMidY()));
    this->number->setAnchorPoint(Vec2(0,0.5));
    this->number->setGlobalZOrder(0);
    
    this->name = MainMenu::CreateLabel("New Level", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->name->setPosition(Vec2(number->getBoundingBox().getMaxX(), background->getBoundingBox().getMidY()));
    this->name->setAnchorPoint(Vec2(0,0.5));
    this->name->setGlobalZOrder(0);
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,0));
    menu->setPosition(0,0);
}
void LevelMenuItem::SetEnabled(bool is){
    menu->setEnabled(is);
    menu->setVisible(is);
    name->setVisible(is);
    number->setVisible(is);
    favorites->setVisible(is);
}
void LevelMenuItem::SetOrder(int){

}
void LevelMenuItem::SetFavorited(bool i){
    if(i){
        int count = std::atoi(favorites->getString().c_str());
        favorites->setString(std::to_string(count));
        favorited->setNormalImage(Sprite::create("FavoriteOn.png"));
    }
    else{
        int count = std::atoi(favorites->getString().c_str());
        favorites->setString(std::to_string(count));
        favorited->setNormalImage(Sprite::create("FavoriteOff.png"));
    }
}
void LevelMenuItem::SetLevel(Level* lvl, int page){
    level = lvl;
    myName = lvl->GetName();
    myPath = lvl->GetPath();
    name->setString(myName);
    number->setString(std::to_string(favorited->getTag() + (page*4) +1) + ") ");
    name->setScale(1.0);
    while(name->getBoundingBox().size.width >= 320){
        name->setScaleX(name->getScaleX() - 0.05);
        name->setScaleY(name->getScaleY() - 0.05);
    }
    this->name->setPosition(Vec2(number->getBoundingBox().getMaxX(), background->getBoundingBox().getMidY()));
    favorites->setString(std::to_string(lvl->GetFavorites()));
    SetFavorited(lvl->GetIsFavorited());
    printf("\nSet up a custom leve with %d favorites that %d favorited.", lvl->GetFavorites(), lvl->GetIsFavorited());
}
void LevelMenuItem::SetTag(int x){
    favorited->setTag(x);
    background->setTag(x);
    if(myName != "")
        name->setString(std::to_string(x) + ") " + myName);
}