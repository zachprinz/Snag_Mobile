//
//  MainMenu.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#include "MainMenu.h"
#include "Game.h"
#include "LevelEditor.h"
#include "LevelSelect.h"
#include "LogInMenu.h"
#include "NDKHelper/NDKHelper.h"
#include "String.h"

USING_NS_CC;
#define MAX_SCALE 1.5;
#define NATIVE_WIDTH 1704;
#define NATIVE_HEIGHT 960;

Scene* MainMenu::myScene;
Vec2 MainMenu::screenSize;
float MainMenu::minScreenScale;
Vec2 MainMenu::screenScale;
MainMenu* MainMenu::Instance;
float MainMenu::transitionTime = 0.5;

Scene* MainMenu::createScene(){
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
    auto background = MainMenu::CreateButton("MMBackground.png", Vec2(0,1), Vec2(0,0));
    if(visibleSize.width <= ((Sprite*)background->getNormalImage())->getTextureRect().size.width && visibleSize.height <= ((Sprite*)background->getNormalImage())->getTextureRect().size.height){
        background->setNormalImage(Sprite::create("MMBackground.png", Rect(0,0,visibleSize.width, visibleSize.height)));
    }
    if(background->getScaleX() < 1.0)
        background->setScaleX(1.0);
    if(background->getScaleY() < 1.0)
        background->setScaleY(1.0);
    background->setPositionZ(-2);
    this->addChild(background,1);
    auto ballAndHook = MainMenu::CreateButton("MMHookAndUser.png", Vec2(0,0),Vec2(0,1));
    ballAndHook->setAnchorPoint(Vec2(0.5,ballAndHook->getAnchorPoint().y));
    ballAndHook->setPosition(Vec2(background->getBoundingBox().getMidX(), (1.0-0.07)*screenSize.y));
    menuItems.pushBack(ballAndHook);
    
    auto playButton = MainMenu::CreateButton("MMPlay.png", this, menu_selector(MainMenu::playButtonCallback), Vec2(0.5, 1.0-0.69), Vec2(1,1));
    playButton->setAnchorPoint(Vec2(0.5,1));
    menuItems.pushBack(playButton);
    auto title = MainMenu::CreateButton("MMSnag.png", Vec2(0,0), Vec2(0,1));
    title->setAnchorPoint(Vec2(0.5,title->getAnchorPoint().y));
    title->setPosition(Vec2(background->getBoundingBox().getMidX(), (1.0-0.05)*screenSize.y));
    menuItems.pushBack(title);
    
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    notice = Label::createWithBMFont("dimbo.fnt", "Searching for an account", TextHAlignment::CENTER);
    notice->setScale(1);
    notice->setVisible(false);
    notice->setPosition(Vec2(visibleSize.width /2.0, visibleSize.height / 2.0));
    notice->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(notice,1);
    
    //-- Set Up Newtork Stuff --//
    NDKHelper::addSelector("MainMenu", "userCheckCallback", CC_CALLBACK_2(MainMenu::userCheckCallback, this), this);
    //--------------------------//
    
    return true;
}

void MainMenu::playButtonCallback(Ref* pSender){
    menu->setVisible(false);
    notice->setVisible(true);
    checkForUser();
}
void MainMenu::checkForUser(){
    sendMessageWithParams("checkForUser", Value());
}
void MainMenu::userCheckCallback(Node* sender, Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        ValueMap valueMap = data.asValueMap();
        std::string response = valueMap["responce"].asString();
        if(response.compare("true") == 0){
            printf("Found a user account for this device!");
            goToLevelSelect();
        }
        else{
            printf("Couldn't find a user account for this device.");
            goToLogIn();
        }
    }
}
bool MainMenu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
};
void MainMenu::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){

};
void MainMenu::goToLevelSelect(){
    if(LevelSelect::myScene == NULL){
        auto scene = LevelSelect::createScene();
        auto toLevelSelectTransition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(toLevelSelectTransition);
        LevelSelect::Instance->Refresh();
    }
    else{
        Director::getInstance()->pushScene(LevelSelect::myScene);
        LevelSelect::Instance->Refresh();
    }
}
void MainMenu::goToLogIn(){
    if(LogInMenu::myScene == NULL){
        auto scene = LogInMenu::createScene();
        Director::getInstance()->pushScene(scene);
    }
    else{
        Director::getInstance()->pushScene(LogInMenu::myScene);
    }
}
MenuItemImage* MainMenu::CreateButton(std::string imagePath, Ref* ref, SEL_MenuHandler callback, Vec2 pos, Vec2 anchors){
    MenuItemImage* temp = MenuItemImage::create(imagePath, imagePath, ref, callback);
    Vec2 tempScale = GetAdjustedScale(pos, anchors);
    temp->setPosition(GetAdjustedPosition(pos, anchors, tempScale));
    temp->setScale(tempScale.x, tempScale.y);
    temp->setAnchorPoint(Point(0,1));
    temp->retain();
    printf("\nCreated a button at position (%f, %f)", temp->getPosition().x, temp->getPosition().y);
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
    temp->setColor(Color3B::BLACK);
    temp->retain();
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
