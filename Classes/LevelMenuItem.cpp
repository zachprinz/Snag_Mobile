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

LevelMenuItem::LevelMenuItem(std::string name, Size visibleSize, int x2, int y2){
    float yPositions2[] = {0.212,0.452,0.68};
    float xPosition2 = 0.021;
    float x = xPosition2;
    float y = yPositions2[count];
    count++;
    Vector<MenuItem*> menuItems;
    
    background = MainMenu::CreateButton("LevelSelectLevelBackground.png", LevelSelect::myScene, menu_selector(LevelSelect::editCallback), Vec2(x,1.0-y), Vec2(0,0));
    background->setScale(background->getScaleX()*0.99, background->getScaleY());
    background->setGlobalZOrder(1);
    background->setEnabled(false);
    menuItems.pushBack(background);
    
    edit = MainMenu::CreateButton("LevelSelectEdit.png", LevelSelect::myScene, menu_selector(LevelSelect::editCallback), Vec2(x+0.848, 1.0-(y+0.022)), Vec2(1,0));
    edit->setGlobalZOrder(2);
    menuItems.pushBack(edit);
    
    highscores = MainMenu::CreateButton("LevelSelectScores.png", LevelSelect::myScene, menu_selector(LevelSelect::highscoresCallback), Vec2(x+0.645, 1.0-(y+0.022)), Vec2(1,0));
    highscores->setGlobalZOrder(2);
    menuItems.pushBack(highscores);
    
    play = MainMenu::CreateButton("LevelSelectPlay.png", LevelSelect::myScene, menu_selector(LevelSelect::playCallback), Vec2(x+0.747, 1.0-(y+0.022)), Vec2(1,0));
    play->setGlobalZOrder(2);
    menuItems.pushBack(play);
    
    this->name = MainMenu::CreateLabel("New Level", Vec2(x+0.048,1.0-(y+0.031)), Vec2(0,0));
    this->name->setGlobalZOrder(2);
    
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