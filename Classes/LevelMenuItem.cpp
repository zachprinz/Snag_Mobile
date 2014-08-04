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
    
    favorited = MainMenu::CreateButton("LSFavCount.png", LevelSelect::Instance, menu_selector(LevelSelect::favoriteCallback), Vec2(x+0.00, 1.0-(y+0.00)), Vec2(1,0));
    favorited->setPosition(Vec2(background->getBoundingBox().getMaxX() - (24*MainMenu::screenScale.y), background->getBoundingBox().getMidY()));
    favorited->setAnchorPoint(Vec2(1,0.5));
    favorited->setGlobalZOrder(0);
    menuItems.pushBack(favorited);
    
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
}
void LevelMenuItem::SetOrder(int){

}
void LevelMenuItem::SetLevel(Level* lvl, int page){
    level = lvl;
    myName = lvl->GetName();
    myPath = lvl->GetPath();
    name->setString(myName);
    name->setString(std::to_string(favorited->getTag() + (page*4) +1) + ") " + myName);
}
void LevelMenuItem::SetTag(int x){
    favorited->setTag(x);
    background->setTag(x);
    if(myName != "")
        name->setString(std::to_string(x) + ") " + myName);
}