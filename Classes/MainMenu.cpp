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
#include "LevelSelect.h"

USING_NS_CC;
#define MAX_SCALE 1.5;
#define NATIVE_WIDTH 1704;
#define NATIVE_HEIGHT 960;

Scene* MainMenu::myScene;
Vec2 MainMenu::screenSize;
float MainMenu::minScreenScale;
Vec2 MainMenu::screenScale;
MainMenu* MainMenu::Instance;

Scene* MainMenu::createScene()
{
    auto scene = Scene::create();
    auto layer = MainMenu::create();
    myScene = scene;
    Instance = layer;
    scene->addChild(layer);
    return scene;
}

bool MainMenu::init()
{
    if(!Layer::init()){
        return false;
    }
    Instance = this;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Vector<MenuItem*> menuItems;
    screenSize.x = visibleSize.width;
    screenSize.y = visibleSize.height;
    Vec2 maxSize(1704, 960); //TODO Replace with defined Native Sizes
    Vec2 adjustedScale = Vec2(visibleSize.width / maxSize.x, visibleSize.height / maxSize.y);
    screenScale = adjustedScale;
    minScreenScale = adjustedScale.x;
    if(adjustedScale.y < adjustedScale.x)
        minScreenScale = adjustedScale.y;
    
    auto background = MainMenu::CreateButton("MainMenu.png", Vec2(0,1), Vec2(0,0));
    background->setPositionZ(-2);
    menuItems.pushBack(background);
    
    auto playButton = MainMenu::CreateButton("MainMenuPlay.png", this, menu_selector(MainMenu::playButtonCallback), Vec2(0.5, 1.0-0.66), Vec2(1,1));
    playButton->setAnchorPoint(Vec2(0.5,1));
    menuItems.pushBack(playButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    return true;
}

void MainMenu::playButtonCallback(Ref* pSender){
    if(LevelSelect::myScene == NULL){
        Board::Print("Play button click registered.");
        auto scene = LevelSelect::createScene();
        Director::getInstance()->pushScene(scene);
        LevelSelect::Instance->Refresh();
    }
    else{
        Director::getInstance()->pushScene(LevelSelect::myScene);
        LevelSelect::Instance->Refresh();
    }
}
bool MainMenu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void MainMenu::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
}

MenuItemImage* MainMenu::CreateButton(std::string imagePath, Ref* ref, SEL_MenuHandler callback, Vec2 pos, Vec2 anchors){
    MenuItemImage* temp = MenuItemImage::create(imagePath, imagePath, ref, callback);
    Vec2 tempScale = GetAdjustedScale(pos, anchors);
    temp->setPosition(GetAdjustedPosition(pos, anchors, tempScale));
    temp->setScale(tempScale.x, tempScale.y);
    temp->setAnchorPoint(Point(0,1));
    temp->retain();
    printf("Created a button at position (%f, %f)", temp->getPosition().x, temp->getPosition().y);
    return temp;
};
MenuItemImage* MainMenu::CreateButton(std::string imagePath, Vec2 pos, Vec2 anchors){
    MenuItemImage* temp = MainMenu::CreateButton(imagePath, MainMenu::Instance, menu_selector(MainMenu::emptyCallback), pos, anchors);
    temp->setEnabled(false);
    return temp;
};

Label* MainMenu::CreateLabel(std::string text, Vec2 pos, Vec2 anchors){
    Label* temp = Label::createWithBMFont("dimbo.fnt", text, TextHAlignment::CENTER);
    Vec2 tempScale = GetAdjustedScale(pos, anchors);
    temp->setPosition(GetAdjustedPosition(pos, anchors, tempScale));
    temp->setAnchorPoint(Point(0.5,0.5));
    temp->retain();
    temp->setColor(Color3B(0.0,0.0,0.0));
    //temp->setFontScale(tempScale);
    return temp;
};
Vec2 MainMenu::GetAdjustedScale(Vec2 pos, Vec2 anchors){
    if(anchors.x == 0 && anchors.y == 0)
        return Vec2(screenScale.x, screenScale.y);
    float tempFloat = MAX_SCALE;
    if(minScreenScale > tempFloat)
        return Vec2(tempFloat, tempFloat);
    return Vec2(minScreenScale, minScreenScale);
};
Vec2 MainMenu::GetAdjustedPosition(Vec2 pos, Vec2 anchors, Vec2 scale){
    float nativeWidth = NATIVE_WIDTH;
    float nativeHeight = NATIVE_HEIGHT;
    Vec2 anchorCorners(-1,-1);
    if(anchors.x == 1){
        anchorCorners.x = 0;
        if(pos.x > 0.5)
            anchorCorners.x = 1;
    }
    if(anchors.y == 1){
        anchorCorners.y = 0;
        if(pos.y > 0.5)
            anchorCorners.y = 1;
    }
    //Find X Position//
    float baseX = pos.x * nativeWidth;
    if(anchors.x == 1){
        if(anchorCorners.x == 1){
            baseX = nativeWidth - baseX;
        }
        baseX *= scale.x;
        if(anchorCorners.x == 1)
            baseX = screenSize.x - baseX;
    }
    if(anchors.x == 0)
        baseX *= screenScale.x;
    ///////////////////
    //Find Y Position//
    float baseY = pos.y * nativeHeight;
    if(anchors.y == 1){
        if(anchorCorners.y == 1){
            baseY = nativeHeight - baseY;
        }
        baseY *= scale.y;
        if(anchorCorners.y == 1)
            baseY = screenSize.y - baseY;
    }
    if(anchors.y == 0)
        baseY *= screenScale.y;
    ///////////////////
    return Vec2(baseX, baseY);
};

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
