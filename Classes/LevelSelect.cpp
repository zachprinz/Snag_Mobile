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
    currentLevelSet = LEVELS_LOCAL;
    
    onlineLevelsButton = MenuItemImage::create("LevelSelectOnlineButton.png", "LevelSelectOnlineButton.png", this, menu_selector(LevelSelect::onlineCallback));
    float availableWidth = visibleSize.width - 155 * MainMenu::screenScale.x;
    onlineLevelsButton->setGlobalZOrder(-1);
    onlineLevelsButton->setPosition(Point(availableWidth / 2.0 + 20, visibleSize.height - 20 - 10));
    onlineLevelsButton->setAnchorPoint(Point(0.5,1.0));
    
    float tabWidth = MainMenu::screenScale.x * 566;
    
    localLevelsButton = MenuItemImage::create("LevelSelectLocalButton.png", "LevelSelectLocalButton.png", this, menu_selector(LevelSelect::localCallback));
    localLevelsButton->setGlobalZOrder(-1);
    localLevelsButton->setPosition(Point(21, visibleSize.height - 20 + 10));
    localLevelsButton->setAnchorPoint(Point(0.0,1.0));

    
    myLevelsButton = MenuItemImage::create("LevelSelectMyLevelsButton.png", "LevelSelectMyLevelsButton.png", this, menu_selector(LevelSelect::myLevelsCallback));
    myLevelsButton->setGlobalZOrder(-1);
    myLevelsButton->setPosition(Point(availableWidth + 10, visibleSize.height - 20 - 10));
    myLevelsButton->setAnchorPoint(Point(1.0,1.0));
    
    float tabHeight = 125 * MainMenu::screenScale.y;
    
    auto scrollUpButton = MenuItemImage::create("LevelSelectScrollUp.png", "LevelSelectScrollUp.png", this, menu_selector(LevelSelect::scrollUpCallback));
    scrollUpButton->setGlobalZOrder(1);
    scrollUpButton->setPosition(Point(visibleSize.width / 2.0, visibleSize.height - tabHeight - 22));//(60 * MainMenu::screenScale.y)));
    scrollUpButton->setAnchorPoint(Point(0.5,1.0));
    
    auto scrollDownButton = MenuItemImage::create("LevelSelectScrollDown.png", "LevelSelectScrollDown.png", this, menu_selector(LevelSelect::scrollDownCallback));
    scrollDownButton->setGlobalZOrder(1);
    scrollDownButton->setPosition(Point(visibleSize.width / 2.0, 12));
    scrollDownButton->setAnchorPoint(Point(0.5,0.0));
    
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelSelect::homeCallback));
    homeButton->setGlobalZOrder(1);
    homeButton->setPosition(Point(visibleSize.width - 10, visibleSize.height - 10));
    homeButton->setAnchorPoint(Point(1.0,1.0));
    
    levelsBackground = MenuItemImage::create("LevelSelectBackground.png", "LevelSelectBackground.png", this, menu_selector(LevelSelect::homeCallback));;//Sprite::create("LevelSelectBackground.png");
    levelsBackground->setEnabled(false);
    levelsBackground->setGlobalZOrder(0);
    levelsBackground->setPosition(Point(0,-10));

    auto background = Sprite::create("background.png");
    background->setGlobalZOrder(-2);
    background->setPosition(Point(0,0));
    background->setAnchorPoint(Point(0,0));
    this->addChild(background);
    levelsBackground->setAnchorPoint(Point(0,0));

    scrollUpButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    scrollDownButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    onlineLevelsButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    localLevelsButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    myLevelsButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    levelsBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    homeButton->setScale(MainMenu::screenScale.x * 0.75, MainMenu::screenScale.y * 0.75);
    background->setScale(visibleSize.width / background->getTexture()->getPixelsWide(), visibleSize.height / background->getTexture()->getPixelsHigh());
    
    menuItems.pushBack(onlineLevelsButton);
    menuItems.pushBack(myLevelsButton);
    menuItems.pushBack(localLevelsButton);
    menuItems.pushBack(levelsBackground);
    menuItems.pushBack(scrollUpButton);
    menuItems.pushBack(scrollDownButton);
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    for(int x = 0; x < 3; x++){
        LevelMenuItem* lvl = new LevelMenuItem("new map", visibleSize, visibleSize.width / 2.0, visibleSize.height - tabHeight - tabHeight - 55 - ((203 * MainMenu::screenScale.y + 5) * x) - 5);
        lvl->SetTag(x);
        AddLevel(lvl);
    }
    
    return true;
}

void LevelSelect::editCallback(Ref*){

}
void LevelSelect::playCallback(Object* sender){
    Level* tempLvl;
    int levelSelected = ((MenuItem*)(sender))->getTag();
    switch(LevelSelect::Instance->currentLevelSet){
        case LEVELS_MY:
            tempLvl = (Board::Instance->myLevels[levelSelected]);
            break;
        case LEVELS_LOCAL:
            tempLvl = (Board::Instance->localLevels[levelSelected]);
            break;
        case LEVELS_ONLINE:
            tempLvl = (Board::Instance->onlineLevels[levelSelected]);
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
    
    
    /*if(HelloWorld::myScene == NULL){
        Board::Print("Play button click registered.");
        auto scene = HelloWorld::createScene();
        Director::getInstance()->pushScene(scene);

    }
    else{
        Director::getInstance()->pushScene(HelloWorld::myScene);
        int levelSelected = ((MenuItem*)(sender))->getTag();
        switch(LevelSelect::Instance->currentLevelSet){
            case LEVELS_MY:
                Board::Instance->Reset(Board::Instance->myLevels[levelSelected]);
                break;
            case LEVELS_LOCAL:
                Board::Instance->Reset(Board::Instance->localLevels[levelSelected]);
                break;
            case LEVELS_ONLINE:
                Board::Instance->Reset(Board::Instance->onlineLevels[levelSelected]);
                break;
        }
    }*/
};
void LevelSelect::highscoresCallback(Ref*){

};
void LevelSelect::scrollUpCallback(Ref*){

};
void LevelSelect::scrollDownCallback(Ref*){

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
            myLevelsButton->setPosition(myLevelsButton->getPosition().x, myLevelsButton->getPosition().y - 20);
            break;
        case LEVELS_LOCAL:
            localLevelsButton->setPosition(localLevelsButton->getPosition().x, localLevelsButton->getPosition().y - 20);
            break;
        case LEVELS_ONLINE:
            onlineLevelsButton->setPosition(onlineLevelsButton->getPosition().x, onlineLevelsButton->getPosition().y - 20);
            break;
    }
    currentLevelSet = newSet;
    switch(currentLevelSet){
        case LEVELS_MY:
            myLevelsButton->setPosition(myLevelsButton->getPosition().x, myLevelsButton->getPosition().y + 20);
            break;
        case LEVELS_LOCAL:
            localLevelsButton->setPosition(localLevelsButton->getPosition().x, localLevelsButton->getPosition().y + 20);
            break;
        case LEVELS_ONLINE:
            onlineLevelsButton->setPosition(onlineLevelsButton->getPosition().x, onlineLevelsButton->getPosition().y + 20);
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