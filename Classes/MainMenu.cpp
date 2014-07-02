//
//  MainMenu.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#include "MainMenu.h"
#include "Board.h"
#include "HelloWorldScene.h"
#include "LevelEditor.h"

USING_NS_CC;

Scene* MainMenu::myScene;
Vec2 MainMenu::screenScale;

Scene* MainMenu::createScene()
{
    auto scene = Scene::create();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto layer = MainMenu::create();
    myScene = scene;
    
    scene->addChild(layer);
    
    return scene;
}

bool MainMenu::init()
{
    if(!Layer::init()){
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    //auto listener = EventListenerTouchOneByOne::create();
    //listener->setSwallowTouches(true);
    //listener->onTouchBegan = CC_CALLBACK_2(MainMenu::onTouchBegan, this);
    //listener->onTouchEnded = CC_CALLBACK_2(MainMenu::onTouchEnded, this);
    
    //_eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto playButton = MenuItemImage::create("MainMenuPlay.png", "MainMenuPlay.png", this, menu_selector(MainMenu::playButtonCallback));
    playButton->setPosition(Point(57 + (visibleSize.width - 100) / 4.0,155));
    menuItems.pushBack(playButton);
    auto levelCreatorButton = MenuItemImage::create("MainMenuLevelCreator.png", "MainMenuLevelCreator.png", this, menu_selector(MainMenu::levelCreatorButtonCallback));
    levelCreatorButton->setPosition(Point(52 + ((visibleSize.width - 100) / 4.0) * 3,155));
    menuItems.pushBack(levelCreatorButton);
    
    auto background = Sprite::create("MainMenu.png");
    background->setAnchorPoint(Point(0,0));
    Vec2 adjustedScale = Vec2(visibleSize.width / background->getTexture()->getPixelsWide(), visibleSize.height / background->getTexture()->getPixelsHigh());
    screenScale = adjustedScale;
    background->setScale(adjustedScale.x,adjustedScale.y);
    playButton->setScale(adjustedScale.x, adjustedScale.y);
    levelCreatorButton->setScale(adjustedScale.x, adjustedScale.y);
    background->setPositionZ(-2);
    this->addChild(background);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    return true;
}

void MainMenu::playButtonCallback(Ref* pSender){
    if(HelloWorld::myScene == NULL){
        Board::Print("Play button click registered.");
        auto scene = HelloWorld::createScene();
        Director::getInstance()->pushScene(scene);
    }
    else{
        Director::getInstance()->pushScene(HelloWorld::myScene);
    }
}

void MainMenu::levelCreatorButtonCallback(Ref* pSender){
    if(LevelEditor::myScene == NULL){
        auto scene = LevelEditor::createScene();
        Director::getInstance()->pushScene(scene);
    }
    else{
        Director::getInstance()->pushScene(LevelEditor::myScene);
    }
}

bool MainMenu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void MainMenu::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
}

void MainMenu::menuCloseCallback(Ref* pSender)
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
