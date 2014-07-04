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
    
    auto onlineLevelsButton = MenuItemImage::create("LevelSelectOnlineButton.png", "LevelSelectOnlineButton.png", this, menu_selector(LevelSelect::onlineCallback));
    float availableWidth = visibleSize.width - 155 * MainMenu::screenScale.x;
    onlineLevelsButton->setPosition(Point(availableWidth / 2.0, visibleSize.height));
    onlineLevelsButton->setAnchorPoint(Point(0.5,1.0));
    menuItems.pushBack(onlineLevelsButton);
    
    float tabWidth = MainMenu::screenScale.x * 566;
    
    auto localLevelsButton = MenuItemImage::create("LevelSelectLocalButton.png", "LevelSelectLocalButton.png", this, menu_selector(LevelSelect::localCallback));
    localLevelsButton->setPosition(Point(0, visibleSize.height));
    localLevelsButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(localLevelsButton);
    
    auto myLevelsButton = MenuItemImage::create("LevelSelectMyLevelsButton.png", "LevelSelectMyLevelsButton.png", this, menu_selector(LevelSelect::myLevelsCallback));
    myLevelsButton->setPosition(Point(availableWidth + 11, visibleSize.height));
    myLevelsButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(myLevelsButton);
    
    float tabHeight = 125 * MainMenu::screenScale.y;
    
    auto scrollUpButton = MenuItemImage::create("LevelSelectScrollUp.png", "LevelSelectScrollUp.png", this, menu_selector(LevelSelect::scrollUpCallback));
    scrollUpButton->setPosition(Point(visibleSize.width / 2.0, visibleSize.height - tabHeight - 15));//(60 * MainMenu::screenScale.y)));
    scrollUpButton->setAnchorPoint(Point(0.5,1.0));
    menuItems.pushBack(scrollUpButton);
    
    auto scrollDownButton = MenuItemImage::create("LevelSelectScrollDown.png", "LevelSelectScrollDown.png", this, menu_selector(LevelSelect::scrollDownCallback));
    scrollDownButton->setPosition(Point(visibleSize.width / 2.0, 15));
    scrollDownButton->setAnchorPoint(Point(0.5,0.0));
    menuItems.pushBack(scrollDownButton);
    
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelSelect::homeCallback));
    homeButton->setPosition(Point(visibleSize.width - 5, visibleSize.height - 5));
    homeButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(homeButton);
    
    
    
    onlineLevelsBackground = Sprite::create("LevelSelectOnline.png");
    myLevelsBackground = Sprite::create("LevelSelectMyLevels.png");
    localLevelsBackground = Sprite::create("LevelSelectLocal.png");
    onlineLevelsBackground->setPosition(Point(0,-5));
    myLevelsBackground->setPosition(Point(0,-5));
    localLevelsBackground->setPosition(Point(0,-5));
    auto background = Sprite::create("background.png");
    background->setPosition(Point(0,0));
    background->setAnchorPoint(Point(0,0));
    this->addChild(background);
    background->setPositionZ(-2);
    
    onlineLevelsBackground->setAnchorPoint(Point(0,0));
    myLevelsBackground->setAnchorPoint(Point(0,0));
    localLevelsBackground->setAnchorPoint(Point(0,0));
    this->addChild(onlineLevelsBackground);
    this->addChild(myLevelsBackground);
    this->addChild(localLevelsBackground);
    onlineLevelsBackground->setVisible(false);
    myLevelsBackground->setVisible(false);

    scrollUpButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    scrollDownButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    onlineLevelsButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    localLevelsButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    myLevelsButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    localLevelsBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    myLevelsBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    onlineLevelsBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    homeButton->setScale(MainMenu::screenScale.x * 0.75, MainMenu::screenScale.y * 0.75);
    background->setScale(visibleSize.width / background->getTexture()->getPixelsWide(), visibleSize.height / background->getTexture()->getPixelsHigh());
    
    for(int x = 0; x < 3; x++){
        LevelMenuItem* lvl = new LevelMenuItem("new map", visibleSize, visibleSize.width / 2.0, visibleSize.height - tabHeight - tabHeight - 55 - ((203 * MainMenu::screenScale.y + 5) * x));
        lvl->SetTag(x);
        menuItems.pushBack(lvl->edit);
        menuItems.pushBack(lvl->highscores);
        menuItems.pushBack(lvl->play);
        this->addChild(lvl->name);
        AddLevel(lvl);
    }
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    return true;
}

void LevelSelect::editCallback(Ref*){

}
void LevelSelect::playCallback(Object* sender){
    if(HelloWorld::myScene == NULL){
        Board::Print("Play button click registered.");
        auto scene = HelloWorld::createScene();
        Director::getInstance()->pushScene(scene);
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
    }
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
    localLevelsBackground->setVisible(true);
    onlineLevelsBackground->setVisible(false);
    myLevelsBackground->setVisible(false);
    currentLevelSet = LEVELS_LOCAL;
    for(int x = 0; x < 3; x++){
        if(Board::localLevels.size() > x){
            levels[x]->SetEnabled(true);
            levels[x]->SetName(Board::localLevelNames[x].c_str());
            levels[x]->SetPath(Board::localLevels[x].c_str());
        }
        else
            levels[x]->SetEnabled(false);
    }
    
    LoadLevels();
};
void LevelSelect::myLevelsCallback(Ref*){
    localLevelsBackground->setVisible(false);
    onlineLevelsBackground->setVisible(false);
    myLevelsBackground->setVisible(true);
    currentLevelSet = LEVELS_MY;
    
    tinyxml2::XMLDocument myLevelsDoc;
    myLevelsDoc.LoadFile(FileUtils::getInstance()->fullPathForFilename("MyLevels.xml").c_str());
    tinyxml2::XMLElement* level = myLevelsDoc.FirstChildElement();
    /*for(int x = 0; x < 3; x++){
        if(level != NULL){
            levels[x]->SetEnabled(true);
            levels[x]->SetName(level->Attribute("name"));
            levels[x]->SetPath(level->Attribute("path"));
        }
        else
            levels[x]->SetEnabled(false);
        level = level->NextSiblingElement();
    }*/
    
    for(int x = 0; x < 3; x++){
        if(Board::myLevels.size() > x){
         levels[x]->SetEnabled(true);
         levels[x]->SetName(Board::myLevelNames[x].c_str());
         levels[x]->SetPath(Board::myLevels[x].c_str());
     }
     else
        levels[x]->SetEnabled(false);
     }
    
    LoadLevels();
};
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
    localLevelsBackground->setVisible(false);
    onlineLevelsBackground->setVisible(true);
    myLevelsBackground->setVisible(false);
    currentLevelSet = LEVELS_ONLINE;
    
    for(int x = 0; x < 3; x++){
        if(Board::onlineLevels.size() > x){
            levels[x]->SetEnabled(true);
            levels[x]->SetName(Board::onlineLevelNames[x].c_str());
            levels[x]->SetPath(Board::onlineLevels[x].c_str());
        }
        else
            levels[x]->SetEnabled(false);
    }
    
    LoadLevels();
};

void LevelSelect::AddLevel(LevelMenuItem* newLevel){
    levels.push_back(newLevel);
    this->addChild(newLevel->background);
    //this->addChild(newLevel->highscores);
    //this->addChild(newLevel->play);
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