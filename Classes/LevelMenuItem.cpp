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

LevelMenuItem::LevelMenuItem(std::string name, Size visibleSize, int x, int y){
    //cocos2d::MenuItemImage* edit;
    //cocos2d::MenuItemImage* play;
    //cocos2d::MenuItemImage* highscores;
    //cocos2d::Sprite* background;
    //cocos2d::MenuItemFont* name;
    background = Sprite::create("LevelSelectLevelBackground.png");
    float backgroundWidth = background->getTexture()->getPixelsWide() * MainMenu::screenScale.x;
    float x2 = x - backgroundWidth / 2.0;
    
    edit = MenuItemImage::create("LevelSelectEdit.png", "LevelSelectEdit.png", LevelSelect::myScene, menu_selector(LevelSelect::editCallback));
    edit->setPosition(Point(backgroundWidth * 0.93 + x2, y));
    edit->setAnchorPoint(Point(0.5,0.5));
    
    highscores = MenuItemImage::create("LevelSelectScores.png", "LevelSelectScores.png", LevelSelect::myScene, menu_selector(LevelSelect::highscoresCallback));
    highscores->setPosition(Point(backgroundWidth * 0.82 + x2, y));
    highscores->setAnchorPoint(Point(0.5,0.5));
    
    play = MenuItemImage::create("LevelSelectPlay.png", "LevelSelectPlay.png", LevelSelect::myScene, menu_selector(LevelSelect::playCallback));
    play->setPosition(Point(backgroundWidth * 0.71 + x2, y));
    play->setAnchorPoint(Point(0.5,0.5));
    
    this->name = Label::createWithBMFont("comfortaa_large.fnt", "New Level", TextHAlignment::CENTER);
    this->name->setPosition(backgroundWidth * 0.35 + x2, y+10);
    this->name->setPositionZ(4);
    
    edit->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    play->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    highscores->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setPosition(Point(x,y));
    background->setAnchorPoint(Point(0.5,0.5));
}
void LevelMenuItem::SetEnabled(bool is){
    edit->setVisible(is);
    edit->setEnabled(is);
    highscores->setVisible(is);
    highscores->setEnabled(is);
    play->setVisible(is);
    play->setEnabled(is);
    background->setVisible(is);
    name->setVisible(is);
}
void LevelMenuItem::SetOrder(int){

}
void LevelMenuItem::SetName(const char* n){
    myName = n;
    this->name->setString(myName);
}
void LevelMenuItem::SetPath(const char* n){
    myPath = n;
}