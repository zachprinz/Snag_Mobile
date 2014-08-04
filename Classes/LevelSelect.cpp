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
    
    currentLevelSet = LEVELS_RISING;
    page = 0;
    
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
    previewTitle = MainMenu::CreateLabel("Select A", Vec2(0,0), Vec2(0,0));
    previewTitle->setGlobalZOrder(0);
    previewTitle->setPosition(Vec2(levelTitle->getBoundingBox().getMidX(), levelTitle->getBoundingBox().getMidY()));
    previewTitle->setVisible(false);
    previewAuthor = MainMenu::CreateLabel("Level", Vec2(0,0), Vec2(0,0));
    previewAuthor->setPosition(Vec2(levelAuthor->getBoundingBox().getMidX(), levelAuthor->getBoundingBox().getMidY()));
    previewAuthor->setGlobalZOrder(0);
    previewAuthor->setVisible(false);
    
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
    this->addChild(previewTitle, 1);
    this->addChild(previewAuthor, 1);
    
    currentLevelsTab = risingLevels;
    
    for(int x = 0; x < 4; x++){
        LevelMenuItem* lvl = new LevelMenuItem(std::to_string(x) + ") New Map");
        lvl->SetTag(x);
        levels.push_back(lvl);
        this->addChild(lvl->menu,1);
        this->addChild(lvl->name,1);
    }
    
    tabHeight = socialLevels->getPosition().y;
    tabHeightSelected = tabHeight - 20;
    currentLevelsTab->setPositionY(tabHeightSelected);
    return true;
}
void LevelSelect::editCallback(Ref*){

}
void LevelSelect::highscoresCallback(Ref*){

}
void LevelSelect::scrollRightCallback(Ref*){
    if(Board::levels[currentLevelSet].size() > (page+1)*4){
        page++;
        LoadLevels();
    }
}
void LevelSelect::scrollLeftCallback(Ref*){
    if(page > 0){
        page--;
        LoadLevels();
    }
}
void LevelSelect::newLevelCallback(Ref*){
    if(LevelEditor::myScene == NULL){
        auto scene = LevelEditor::createScene();
        Director::getInstance()->pushScene(scene);
    }
    else{
        Director::getInstance()->pushScene(LevelEditor::myScene);
    }
}
void LevelSelect::facebookCallback(Ref*){
    
}
void LevelSelect::favoriteCallback(Ref*){
    
}
void LevelSelect::selectCallback(Ref* ref){
    int tag = ((MenuItemImage*)ref)->getTag();
    selectedLevel = levels[tag]->level;
    SetPreview();
}
void LevelSelect::playCallback(Ref* sender){
    if(selectedLevel != NULL){
        if(HelloWorld::myScene == NULL){
            auto scene = HelloWorld::createScene();
            Director::getInstance()->pushScene(scene);
            Board::Instance->Reset(selectedLevel);
        }
        else{
            Director::getInstance()->pushScene(HelloWorld::myScene);
            Board::Instance->Reset(selectedLevel);
        }
    }
}
void LevelSelect::customCallback(Ref*){
    currentLevelSet = LEVELS_CUSTOM;
    LoadLevels();
    SetLevelSetButtons();
}
void LevelSelect::favoritedCallback(Ref*){
    currentLevelSet = LEVELS_FAVORITED;
    LoadLevels();
    SetLevelSetButtons();
}
void LevelSelect::socialCallback(Ref*){
    currentLevelSet = LEVELS_SOCIAL;
    LoadLevels();
    SetLevelSetButtons();
}
void LevelSelect::risingCallback(Ref*){
    currentLevelSet = LEVELS_RISING;
    LoadLevels();
    SetLevelSetButtons();
}
void LevelSelect::SetLevelSetButtons(){
    currentLevelsTab->runAction(MoveTo::create(0.1, Point(currentLevelsTab->getPosition().x, tabHeight)));
    switch(currentLevelSet){
        case LEVELS_RISING:
            currentLevelsTab = risingLevels;
            break;
        case LEVELS_FAVORITED:
            currentLevelsTab = favoritedLevels;
            break;
        case LEVELS_SOCIAL:
            currentLevelsTab = socialLevels;
            break;
        case LEVELS_CUSTOM:
            currentLevelsTab = customLevels;
            break;
    }
    currentLevelsTab->runAction(MoveTo::create(0.1, Point(currentLevelsTab->getPosition().x, tabHeightSelected)));
}
void LevelSelect::Refresh(){
    switch(currentLevelSet){
        case LEVELS_RISING:
            risingCallback(NULL);
            break;
        case LEVELS_FAVORITED:
            favoritedCallback(NULL);
            break;
        case LEVELS_SOCIAL:
            socialCallback(NULL);
            break;
        case LEVELS_CUSTOM:
            customCallback(NULL);
            break;
    }
}
void LevelSelect::LoadLevels(){
    for(int x = 0; x < 4; x++){
        levels[x]->SetEnabled(false);
    }
    if(Board::levels.size() > 0){
        for(int x = page * 4; x < Board::levels[currentLevelSet].size() && x < ((page*4)+4); x++){
            levels[x%4]->SetEnabled(true);
            levels[x%4]->SetLevel(Board::levels[currentLevelSet][x], page);
        }
    }
};
void LevelSelect::SetPreview(){
    previewTitle->setVisible(true);
    previewAuthor->setVisible(true);
    previewTitle->setString(selectedLevel->GetName());
    previewAuthor->setString(selectedLevel->GetAuthor());
}
void LevelSelect::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}