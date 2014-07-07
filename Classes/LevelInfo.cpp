//
//  LevelInfo.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#include "LevelInfo.h"
#include "LevelInfo.h"
#include "Board.h"
#include "HelloWorldScene.h"
#include "LevelEditor.h"
#include "LevelSelect.h"
#include "MainMenu.h"

USING_NS_CC;

Scene* LevelInfo::myScene;

Scene* LevelInfo::createScene(){
    auto scene = Scene::create();
    auto layer = LevelInfo::create();
    myScene = scene;
    scene->addChild(layer);
    return scene;
}

bool LevelInfo::init() {
    if(!Layer::init()){
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto background = Sprite::create("Base.png");
    auto titleBackground = Sprite::create("TitleBackground.png");
    auto authorBackground = Sprite::create("AuthorBackground.png");
    auto levelInfoBackground = Sprite::create("InfoBackground.png");
    auto highscoresBackground = Sprite::create("HighScoresBackground.png");
    auto highScoresLabel = Sprite::create("HighScoresLabel.png");
    background->setAnchorPoint(Point(0,0));
    background->setPosition(0,0);
    titleBackground->setAnchorPoint(Point(0,0));
    titleBackground->setPosition(0,0);
    authorBackground->setAnchorPoint(Point(0,0));
    authorBackground->setPosition(0,0);
    levelInfoBackground->setAnchorPoint(Point(0,0));
    levelInfoBackground->setPosition(0,0);
    highscoresBackground->setAnchorPoint(Point(0,0));
    highscoresBackground->setPosition(0,0);
    highScoresLabel->setAnchorPoint(Point(0,0));
    highScoresLabel->setPosition(0,0);
    
    title = Label::createWithBMFont("dimbo.fnt", "New Level 1", TextHAlignment::CENTER);
    title->setPosition();
    
    author = Label::createWithBMFont("dimbo.fnt", "The Gaate", TextHAlignment::CENTER);
    author->setPosition();
    
    auto playButton = MenuItemImage::create("play.png", "play.png", this, menu_selector(LevelInfo::playButtonCallback));
    playButton->setPosition(Point());
    menuItems.pushBack(playButton);
    
    auto favoriteButton = MenuItemImage::create("FavoriteOn.png", "FavoriteOff.png", this, menu_selector(LevelInfo::favoriteButtonCallback));
    favoriteButton->setPosition(Point());
    menuItems.pushBack(favoriteButton);
    
    auto downloadButton = MenuItemImage::create("DownloadOn.png", "DownloadOff.png", this, menu_selector(LevelInfo::downloadButtonCallback));
    downloadButton->setPosition(Point());
    menuItems.pushBack(downloadButton);
    
    auto highscoresButton = MenuItemImage::create("LevelSelectScores.png", "LevelSelectScores.png", this, menu_selector(LevelInfo::highscoresButtonCallback));
    highscoresButton->setPosition(Point());
    menuItems.pushBack(highscoresButton);
    
    auto editButton = MenuItemImage::create("LevelSelectEdit.png", "LevelSelectEdit.png", this, menu_selector(LevelInfo::editButtonCallback));
    editButton->setPosition(Point());
    menuItems.pushBack(editButton);
    
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelInfo::homeButtonCallback));
    homeButton->setPosition(Point());
    menuItems.pushBack(homeButton);
    
    homeButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    playButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    favoriteButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    editButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    highscoresButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    downloadButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    highScoresLabel->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    authorBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    highscoresBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    titleBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    levelInfoBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);

    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    
    this->addChild(background);
    this->addChild(titleBackground);
    this->addChild(authorBackground);
    this->addChild(highscoresBackground);
    this->addChild(highScoresLabel);
    this->addChild(levelInfoBackground);
    this->addChild(menu, 1);
    this->addChild(title);
    this->addChild(author);
    
    return true;
}
void LevelInfo::homeButtonCallback(Ref*){

};
void LevelInfo::favoriteButtonCallback(Ref*){

};
void LevelInfo::highscoresButtonCallback(Ref*){

};
void LevelInfo::editButtonCallback(Ref*){

};
void LevelInfo::downloadButtonCallback(Ref*){

};
void LevelInfo::playButtonCallback(Ref*){

};
void LevelInfo::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}