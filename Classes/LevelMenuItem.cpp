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

LevelMenuItem::LevelMenuItem(std::string name, Size visibleSize, int x, int y){
    Vector<MenuItem*> menuItems;
    
    Sprite* background2 = Sprite::create("LevelSelectLevelBackground.png");
    float backgroundWidth = background2->getTexture()->getPixelsWide() * MainMenu::screenScale.x;
    float x2 = x - backgroundWidth / 2.0;
    
    background = MenuItemImage::create("LevelSelectLevelBackground.png", "LevelSelectLevelBackground.png", LevelSelect::myScene, menu_selector(LevelSelect::editCallback));
    background->setGlobalZOrder(1);
    background->setEnabled(false);
    background->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setPosition(Point(x,y));
    background->setAnchorPoint(Point(0.5,0.5));
    menuItems.pushBack(background);
    
    edit = MenuItemImage::create("LevelSelectEdit.png", "LevelSelectEdit.png", LevelSelect::myScene, menu_selector(LevelSelect::editCallback));
    edit->setGlobalZOrder(2);
    edit->setPosition(Point(backgroundWidth * 0.93 + x2, y));
    edit->setAnchorPoint(Point(0.5,0.5));
    menuItems.pushBack(edit);
    
    highscores = MenuItemImage::create("LevelSelectScores.png", "LevelSelectScores.png", LevelSelect::myScene, menu_selector(LevelSelect::highscoresCallback));
    highscores->setGlobalZOrder(2);
    highscores->setPosition(Point(backgroundWidth * 0.82 + x2, y));
    highscores->setAnchorPoint(Point(0.5,0.5));
    menuItems.pushBack(highscores);
    
    play = MenuItemImage::create("LevelSelectPlay.png", "LevelSelectPlay.png", LevelSelect::myScene, menu_selector(LevelSelect::playCallback));
    play->setGlobalZOrder(2);
    play->setPosition(Point(backgroundWidth * 0.71 + x2, y));
    play->setAnchorPoint(Point(0.5,0.5));
    menuItems.pushBack(play);
    
    this->name = Label::createWithBMFont("dimbo.fnt", "New Level", TextHAlignment::CENTER);
    this->name->setGlobalZOrder(2);
    this->name->setPosition(backgroundWidth * 0.35 + x2, y+10);

    edit->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    play->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    highscores->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0,0));
    menu->setPosition(0,0);

}
void LevelMenuItem::SetEnabled(bool is){
    menu->setEnabled(is);
    menu->setVisible(is);
    name->setVisible(is);
}
void LevelMenuItem::SetOrder(int){

}
void LevelMenuItem::SetLevel(Level* lvl){
    level = lvl;
    myName = lvl->GetName();
    myPath = lvl->GetPath();
    name->setString(myName);
}
void LevelMenuItem::SetTag(int x){
    play->setTag(x);
}