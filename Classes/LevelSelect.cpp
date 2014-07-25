//
//  LevelSelect.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/3/14.
//
//

#include "LevelSelect.h"
#include "Board.h"
#include "HelloWorldScene.h"
#include "LevelEditor.h"
#include "MainMenu.h"
#include "LevelMenuItem.h"
#include "LevelInfo.h"

USING_NS_CC;

Scene* LevelSelect::myScene;
Vec2 LevelSelect::screenScale;
LevelSelect* LevelSelect::Instance;

Scene* LevelSelect::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LevelSelect::create();
    myScene = scene;
    
    scene->addChild(layer);
    
    return scene;
}

bool LevelSelect::init()
{
    if(!Layer::init()){
        return false;
    }
    Board::customLevel = false;
    Instance = this;
    
    currentLevelSet = LEVELS_LOCAL;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    cocos2d::Vector<MenuItem*> menuItems;
    
    onlineLevelsButton = MainMenu::CreateButton("LevelSelectOnlineButton.png", this, menu_selector(LevelSelect::onlineCallback), Vec2(0.315,1.0-0.05), Vec2(0,0));
    onlineLevelsButton->setGlobalZOrder(-1);
    localLevelsButton = MainMenu::CreateButton("LevelSelectLocalButton.png", this, menu_selector(LevelSelect::localCallback), Vec2(0.013,1.0-0.05), Vec2(0,0));
    localLevelsButton->setGlobalZOrder(-1);
    myLevelsButton = MainMenu::CreateButton("LevelSelectMyLevelsButton.png", this, menu_selector(LevelSelect::myLevelsCallback), Vec2(0.62,1.0-0.05), Vec2(0,0));
    myLevelsButton->setGlobalZOrder(-1);
    auto homeButton = MainMenu::CreateButton("home.png", this, menu_selector(LevelSelect::homeCallback), Vec2(0.9175,1.0-0.01), Vec2(1,1));
    homeButton->setScale(homeButton->getScaleX()*0.85, homeButton->getScaleY()*0.85);
    homeButton->setGlobalZOrder(1);
    levelsBackground = MainMenu::CreateButton("LSB.png", this, menu_selector(LevelSelect::homeCallback), Vec2(0,1.0-0.154), Vec2(0,0));
    levelsBackground->setEnabled(false);
    levelsBackground->setGlobalZOrder(0);
    
    auto background = Sprite::create("background.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Vec2(0,0));
    background->setGlobalZOrder(-2);
    this->addChild(background);
    background->setScale(visibleSize.width / background->getTexture()->getPixelsWide(), visibleSize.height / background->getTexture()->getPixelsHigh());
    
    menuItems.pushBack(onlineLevelsButton);
    menuItems.pushBack(myLevelsButton);
    menuItems.pushBack(localLevelsButton);
    menuItems.pushBack(levelsBackground);
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    for(int x = 0; x < 3; x++){
        LevelMenuItem* lvl = new LevelMenuItem("new map", visibleSize, 0, 0);
        lvl->SetTag(x);
        AddLevel(lvl);
    }
    tabHeight = onlineLevelsButton->getPosition().y;
    tabHeightSelected = tabHeight + 20;
    
    return true;
}

void LevelSelect::editCallback(Ref*){

}
void LevelSelect::playCallback(Object* sender){
    Level* tempLvl;
    int levelSelected = ((MenuItem*)(sender))->getTag();
    switch(LevelSelect::Instance->currentLevelSet){
        case LEVELS_MY:
            tempLvl = (Board::myLevels[levelSelected]);
            break;
        case LEVELS_LOCAL:
            tempLvl = (Board::localLevels[levelSelected]);
            break;
        case LEVELS_ONLINE:
            tempLvl = (Board::onlineLevels[levelSelected]);
            break;
    }
    if(LevelInfo::myScene == NULL){
        Board::Print("Play button click registered.");
        auto scene = LevelInfo::createScene();
        Director::getInstance()->pushScene(scene);
        LevelInfo::Instance->Load(tempLvl);
    }
    else{
        Director::getInstance()->pushScene(LevelInfo::myScene);
        LevelInfo::Instance->Load(tempLvl);
    }
};
void LevelSelect::highscoresCallback(Ref*){

};
void LevelSelect::homeCallback(Ref* ref){
    Director::getInstance()->pushScene(MainMenu::myScene);
}
void LevelSelect::localCallback(Ref*){
    SetLevelSetButtons(LEVELS_LOCAL);
    for(int x = 0; x < 3; x++){
        if(Board::localLevels.size() > x){
            levels[x]->SetEnabled(true);
            levels[x]->SetLevel(Board::localLevels[x]);
        }
        else
            levels[x]->SetEnabled(false);
    }
    
    LoadLevels();
};
void LevelSelect::myLevelsCallback(Ref*){
    SetLevelSetButtons(LEVELS_MY);
    tinyxml2::XMLDocument myLevelsDoc;
    myLevelsDoc.LoadFile(FileUtils::getInstance()->fullPathForFilename("MyLevels.xml").c_str());
    tinyxml2::XMLElement* level = myLevelsDoc.FirstChildElement();
    for(int x = 0; x < 3; x++){
        if(Board::myLevels.size() > x){
         levels[x]->SetEnabled(true);
         levels[x]->SetLevel(Board::myLevels[x]);
     }
     else
        levels[x]->SetEnabled(false);
     }
    
    LoadLevels();
};
void LevelSelect::SetLevelSetButtons(int newSet){
    switch(currentLevelSet){
        case LEVELS_MY:
            myLevelsButton->runAction(MoveTo::create(0.1, Point(myLevelsButton->getPosition().x, tabHeight)));
            break;
        case LEVELS_LOCAL:
            localLevelsButton->runAction(MoveTo::create(0.1, Point(localLevelsButton->getPosition().x, tabHeight)));
            break;
        case LEVELS_ONLINE:
            onlineLevelsButton->runAction(MoveTo::create(0.1, Point(onlineLevelsButton->getPosition().x, tabHeight)));
            break;
    }
    currentLevelSet = newSet;
    switch(currentLevelSet){
        case LEVELS_MY:
            myLevelsButton->runAction(MoveTo::create(0.1, Point(myLevelsButton->getPosition().x, tabHeightSelected)));
            break;
        case LEVELS_LOCAL:
            localLevelsButton->runAction(MoveTo::create(0.1, Point(localLevelsButton->getPosition().x, tabHeightSelected)));
            break;
        case LEVELS_ONLINE:
            onlineLevelsButton->runAction(MoveTo::create(0.1, Point(onlineLevelsButton->getPosition().x, tabHeightSelected)));
            break;
    }
}
void LevelSelect::Refresh(){
    switch(currentLevelSet){
        case LEVELS_MY:
            myLevelsCallback(NULL);
            break;
        case LEVELS_LOCAL:
            localCallback(NULL);
            break;
        case LEVELS_ONLINE:
            onlineCallback(NULL);
            break;
    }
}
void LevelSelect::onlineCallback(Ref*){
    SetLevelSetButtons(LEVELS_ONLINE);
    for(int x = 0; x < 3; x++){
        if(Board::onlineLevels.size() > x){
            levels[x]->SetEnabled(true);
            levels[x]->SetLevel(Board::onlineLevels[x]);
        }
        else
            levels[x]->SetEnabled(false);
    }
    
    LoadLevels();
};

void LevelSelect::AddLevel(LevelMenuItem* newLevel){
    levels.push_back(newLevel);
    this->addChild(newLevel->menu,1);
    this->addChild(newLevel->name);
};
void LevelSelect::LoadLevels(){

};

void LevelSelect::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}