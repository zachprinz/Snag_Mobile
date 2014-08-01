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
    
    risingLevels = MainMenu::CreateButton("LSRisingTab.png", this, menu_selector(LevelSelect::risingCallback), Vec2(0.015,1.0-0.105), Vec2(0,0));
    risingLevels->setGlobalZOrder(0);
    socialLevels = MainMenu::CreateButton("LSSocialTab.png", this, menu_selector(LevelSelect::socialCallback), Vec2(0.151,1.0-0.105), Vec2(0,0));
    socialLevels->setGlobalZOrder(0);
    favoritedLevels = MainMenu::CreateButton("LSFavTab.png", this, menu_selector(LevelSelect::favoritedCallback), Vec2(0.29,1.0-0.105), Vec2(0,0));
    favoritedLevels->setGlobalZOrder(0);
    customLevels = MainMenu::CreateButton("LSCustomTab.png", this, menu_selector(LevelSelect::customCallback), Vec2(0.423,1.0-0.105), Vec2(0,0));
    customLevels->setGlobalZOrder(0);
    auto seperator = MainMenu::CreateButton("LSSeperator.png", Vec2(0.113,1.0-0.822), Vec2(0,0));
    seperator->setGlobalZOrder(0);
    auto leftButton = MainMenu::CreateButton("LSLeft.png", this, menu_selector(LevelSelect::scrollLeftCallback), Vec2(0.016,1.0-0.819), Vec2(0,0));
    leftButton->setGlobalZOrder(0);
    auto rightButton = MainMenu::CreateButton("LSRight.png", this, menu_selector(LevelSelect::scrollRightCallback), Vec2(0.47,1.0-0.819), Vec2(0,0));
    rightButton->setGlobalZOrder(0);
    auto newLevelButton = MainMenu::CreateButton("LSNewLevel.png", this, menu_selector(LevelSelect::newLevelCallback), Vec2(0.114,1.0-0.855), Vec2(0,0));
    newLevelButton->setGlobalZOrder(0);
    auto levelTitle = MainMenu::CreateButton("LSPreviewTitle.png", Vec2(0.585,1.0-0.028), Vec2(0,0));
    levelTitle->setGlobalZOrder(0);
    auto levelAuthor = MainMenu::CreateButton("LSPreviewAuthor.png", Vec2(0.621,1.0-0.163), Vec2(0,0));
    levelAuthor->setGlobalZOrder(0);
    auto previewWindow = MainMenu::CreateButton("LSPreviewPreview.png", Vec2(0.582,1.0-0.303), Vec2(0,0));
    previewWindow->setGlobalZOrder(0);
    auto playButton = MainMenu::CreateButton("LSPlay.png", this, menu_selector(LevelSelect::playCallback), Vec2(0.587,1.0-0.808), Vec2(0,0));
    playButton->setGlobalZOrder(0);
    auto highscoresButton = MainMenu::CreateButton("LSHighscores.png", this, menu_selector(LevelSelect::highscoresCallback), Vec2(0.681,1.0-0.808), Vec2(0,0));
    highscoresButton->setGlobalZOrder(0);
    auto facebookButton = MainMenu::CreateButton("LSFacebook.png", this, menu_selector(LevelSelect::facebookCallback), Vec2(0.789,1.0-0.808), Vec2(0,0));
    facebookButton->setGlobalZOrder(0);
    auto editButton = MainMenu::CreateButton("LSEdit.png", this, menu_selector(LevelSelect::editCallback), Vec2(0.887,1.0-0.808), Vec2(0,0));
    editButton->setGlobalZOrder(0);
    auto selectBackground = MainMenu::CreateButton("LSLevelBackground.png",  Vec2(0.01,1.0-0.206), Vec2(0,0));
    selectBackground->setGlobalZOrder(0);
    auto previewBackground = MainMenu::CreateButton("LSPreviewBackground.png", Vec2(0.574,1.0-0.013), Vec2(0,0));
    previewBackground->setGlobalZOrder(0);
    auto title = MainMenu::CreateButton("LSTitle.png", Vec2(0.144,1.0-0.0), Vec2(0,0));
    title->setGlobalZOrder(0);
    auto background = MainMenu::CreateButton("LSBackground.png", Vec2(0,1.0-0.0), Vec2(0,0));
    background->setGlobalZOrder(0);
    
    menuItems.pushBack(background);
    menuItems.pushBack(previewBackground);
    menuItems.pushBack(risingLevels);
    menuItems.pushBack(customLevels);
    menuItems.pushBack(socialLevels);
    menuItems.pushBack(favoritedLevels);
    menuItems.pushBack(selectBackground);
    menuItems.pushBack(leftButton);
    menuItems.pushBack(rightButton);
    menuItems.pushBack(seperator);
    menuItems.pushBack(newLevelButton);
    menuItems.pushBack(previewWindow);
    menuItems.pushBack(levelTitle);
    menuItems.pushBack(levelAuthor);
    menuItems.pushBack(facebookButton);
    menuItems.pushBack(editButton);
    menuItems.pushBack(playButton);
    menuItems.pushBack(highscoresButton);
    menuItems.pushBack(title);

    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    //for(int x = 0; x < 3; x++){
    //    LevelMenuItem* lvl = new LevelMenuItem("new map", visibleSize, 0, 0);
    ////    lvl->SetTag(x);
    //    AddLevel(lvl);
    //}
    //tabHeight = onlineLevelsButton->getPosition().y;
    //tabHeightSelected = tabHeight + 20;
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
void LevelSelect::customCallback(Ref*){
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
void LevelSelect::favoritedCallback(Ref*){
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
            break;
        case LEVELS_LOCAL:
            //localLevelsButton->runAction(MoveTo::create(0.1, Point(localLevelsButton->getPosition().x, tabHeight)));
            break;
        case LEVELS_ONLINE:
            break;
    }
    currentLevelSet = newSet;
    switch(currentLevelSet){
        case LEVELS_MY:
            break;
        case LEVELS_LOCAL:
            break;
        case LEVELS_ONLINE:
            break;
    }
}
void LevelSelect::Refresh(){
    switch(currentLevelSet){
        case LEVELS_MY:
            //myLevelsCallback(NULL);
            break;
        case LEVELS_LOCAL:
            //localCallback(NULL);
            break;
        case LEVELS_ONLINE:
            //onlineCallback(NULL);
            break;
    }
}
void LevelSelect::socialCallback(Ref*){
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
void LevelSelect::risingCallback(Ref*){
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
};
void LevelSelect::LoadLevels(){

};
void LevelSelect::scrollRightCallback(Ref*){
    
}
void LevelSelect::scrollLeftCallback(Ref*){
    
}
void LevelSelect::newLevelCallback(Ref*){
    
}
void LevelSelect::facebookCallback(Ref*){
    
}

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