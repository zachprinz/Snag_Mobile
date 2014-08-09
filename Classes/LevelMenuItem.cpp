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

LevelMenuItem::LevelMenuItem(std::string name){
    float yPositions[] = {0.22,0.37,0.37 + (.37-.22),0.37 + (2*(0.37-.22))};
    float xPosition = 0.02;
    float x = xPosition;
    float y = yPositions[count];
    count++;
    Vector<MenuItem*> menuItems;
    
    background = MainMenu::CreateButton("LSSingleLevelBackground.png", LevelSelect::Instance, menu_selector(LevelSelect::selectCallback), Vec2(x,1.0-y), Vec2(0,0));
    background->setGlobalZOrder(0);
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
    
    this->name = MainMenu::CreateLabel("New Level", Vec2(x+0.00,1.0-(y+0.00)), Vec2(0,0));
    this->name->setPosition(Vec2(background->getPositionX() + (24*MainMenu::screenScale.x), background->getBoundingBox().getMidY()));
    this->name->setAnchorPoint(Vec2(0,0.5));
    this->name->setGlobalZOrder(0);
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,0));
    menu->setPosition(0,0);
    Board::Print("\nCreated a new Level Menu Item.\n");
}
void LevelMenuItem::SetEnabled(bool is){
    menu->setEnabled(is);
    menu->setVisible(is);
    name->setVisible(is);
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
    name->setString(std::to_string(favorited->getTag() + (page*4) +1) + ") " + myName);
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