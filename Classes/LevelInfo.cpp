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
LevelInfo* LevelInfo::Instance;

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
    titleBackground->setPosition(1.5 * 0.01 * visibleSize.width, visibleSize.height - (0.01 * visibleSize.height * 2.5));
    titleBackground->setAnchorPoint(Point(0.0,1.0));
    authorBackground->setAnchorPoint(Point(0,0));
    authorBackground->setPosition(10.5 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height * 17.2));
    authorBackground->setAnchorPoint(Point(0.0,1.0));
    levelInfoBackground->setAnchorPoint(Point(0,0));
    levelInfoBackground->setPosition(3.3 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height * 31.4));
    levelInfoBackground->setAnchorPoint(Point(0.0,1.0));
    highscoresBackground->setAnchorPoint(Point(0,0));
    highscoresBackground->setPosition(51.5 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height * 18.5));
    highscoresBackground->setAnchorPoint(Point(0.0,1.0));
    highScoresLabel->setAnchorPoint(Point(0,0));
    highScoresLabel->setPosition(51.5 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height * 2.5));
    highScoresLabel->setAnchorPoint(Point(0.0,1.0));
    
    title = Label::createWithBMFont("dimbo.fnt", "New Level 1", TextHAlignment::CENTER);
    title->setPosition(26.5 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height * 9.5));
    title->setAnchorPoint(Point(0.5,0.5));
    
    author = Label::createWithBMFont("dimbo.fnt", "The Gaate", TextHAlignment::CENTER);
    author->setPosition(26.5 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height * 22.9));
    author->setScale(0.85);
    author->setAnchorPoint(Point(0.5,0.5));
    
    auto playButton = MainMenu::CreateButton("LevelEditorPlay.png", this, menu_selector(LevelInfo::playButtonCallback), Vec2(0.015, 1-0.815), Vec2(0,1));
    menuItems.pushBack(playButton);
    
    auto favoriteButton = MenuItemImage::create("FavoriteOn.png", "FavoriteOff.png", this, menu_selector(LevelInfo::favoriteButtonCallback));
    favoriteButton->setAnchorPoint(Point(0.0,1.0));
    favoriteButton->setPosition(Point(11.8 * 0.01 * visibleSize.width, visibleSize.height - (0.01 * visibleSize.height * 81.5)));
    menuItems.pushBack(favoriteButton);
    
    auto downloadButton = MenuItemImage::create("DownloadOn.png", "DownloadOff.png", this, menu_selector(LevelInfo::downloadButtonCallback));
    downloadButton->setAnchorPoint(Point(0.0,1.0));
    downloadButton->setPosition(Point(21.5 * 0.01 * visibleSize.width, visibleSize.height - (0.01 * visibleSize.height * 81.5)));
    menuItems.pushBack(downloadButton);
    
    auto highscoresButton = MenuItemImage::create("LevelSelectScores.png", "LevelSelectScores.png", this, menu_selector(LevelInfo::highscoresButtonCallback));
    highscoresButton->setAnchorPoint(Point(0.0,1.0));
    highscoresButton->setPosition(Point(31.4 * 0.01 * visibleSize.width,visibleSize.height - (0.01 * visibleSize.height *  81.5)));
    menuItems.pushBack(highscoresButton);
    
    auto editButton = MenuItemImage::create("LevelSelectEdit.png", "LevelSelectEdit.png", this, menu_selector(LevelInfo::editButtonCallback));
        editButton->setAnchorPoint(Point(0.0,1.0));
    editButton->setPosition(Point(41.2 * 0.01 * visibleSize.width, visibleSize.height - (0.01 * visibleSize.height * 81.5)));
    menuItems.pushBack(editButton);
    
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelInfo::homeButtonCallback));
    homeButton->setAnchorPoint(Point(0.0,1.0));
    homeButton->setPosition(Point(89.4 * 0.01 * visibleSize.width, visibleSize.height - (0.01 * visibleSize.height * 1.2)));
    menuItems.pushBack(homeButton);
    
    homeButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
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
    
    Instance = this;
    
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
    if(HelloWorld::myScene == NULL){
        auto scene = HelloWorld::createScene();
        Director::getInstance()->pushScene(scene);
        Board::Instance->Reset(currentLvl);
    }
    else{
        Director::getInstance()->pushScene(HelloWorld::myScene);
        Board::Instance->Reset(currentLvl);
    }
};
void LevelInfo::Load(Level* lvl){
    currentLvl = lvl;
    title->setString(currentLvl->GetName());
    author->setString(currentLvl->GetAuthor());
}
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