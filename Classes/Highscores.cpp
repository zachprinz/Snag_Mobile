//
//  Highscores.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 9/12/14.
//
//

#include "Highscores.h"
#include "Game.h"
#include "MainMenu.h"
#include "LevelSelect.h"
#include "NDKHelper/NDKHelper.h"

USING_NS_CC;
#define MAX_SCALE 1.5;
#define NATIVE_WIDTH 1704;
#define NATIVE_HEIGHT 960;

Scene* Highscores::myScene;
Highscores* Highscores::Instance;

Scene* Highscores::createScene(){
    auto scene = Scene::create();
    auto layer = Highscores::create();
    myScene = scene;
    Instance = layer;
    scene->addChild(layer);
    return scene;
}

bool Highscores::init()
{
    if(!Layer::init()){
        return false;
    }
    Instance = this;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto background = MainMenu::CreateButton("MMBackground.png", Vec2(0,1), Vec2(0,0));
    if(visibleSize.width <= ((Sprite*)background->getNormalImage())->getTextureRect().size.width && visibleSize.height <= ((Sprite*)background->getNormalImage())->getTextureRect().size.height){
        background->setNormalImage(Sprite::create("MMBackground.png", Rect(0,0,visibleSize.width, visibleSize.height)));
    }
    if(background->getScaleX() < 1.0)
        background->setScaleX(1.0);
    if(background->getScaleY() < 1.0)
        background->setScaleY(1.0);
    background->setPositionZ(-2);
    menuItems.pushBack(background);
    
    auto mainPanel = MainMenu::CreateButton("HSMainPanel.png", Vec2(0.00,0.829),Vec2(0,0));
    menuItems.pushBack(mainPanel);
    auto userPanel = MainMenu::CreateButton("HSUserPanel.png", Vec2(.009,0.17),Vec2(0,0));
    menuItems.pushBack(userPanel);
    
    auto facebookButton = MainMenu::CreateButton("LSFacebook.png", this, menu_selector(Highscores::facebookButtonCallback), Vec2(0.715,0.17), Vec2(0,0));
    menuItems.pushBack(facebookButton);
    auto playButton = MainMenu::CreateButton("LSPlay.png", this, menu_selector(Highscores::playButtonCallback), Vec2(0.903, 0.17), Vec2(0,0));
    menuItems.pushBack(playButton);
    auto homeButton = MainMenu::CreateButton("home.png", this, menu_selector(Highscores::homeButtonCallback), Vec2(0.9, 0.99), Vec2(0,0));
    menuItems.pushBack(homeButton);
    auto favoriteButton = MainMenu::CreateButton("FavoritedButton.png", this, menu_selector(Highscores::favoriteButtonCallback), Vec2(0.809, 0.17), Vec2(0,0));
    menuItems.pushBack(favoriteButton);
    slider = MainMenu::CreateButton("HSScroller.png", this, menu_selector(Highscores::sliderCallback), Vec2(0.023, 0.247), Vec2(0,0));
    //menuItems.pushBack(slider);
    
    slider2 = cocos2d::ui::Slider::create();
    slider2->loadBarTexture("HSSliderTrack.png");
    slider2->loadSlidBallTextures("HSScroller.png","HSScroller.png","");
    slider2->setCapInsets(Rect(20,0,-20,0));
    slider2->setPosition(Vec2(visibleSize.width * 0.5, visibleSize.height * 0.247));
    slider2->setAnchorPoint(Point(0.5,1));
    slider2->setScale(visibleSize.width / 1704, visibleSize.height / 960);
    slider2->setPercent(50);
    
    scrollview = cocos2d::ui::ScrollView::create();
    scrollview->setContentSize(Size(mainPanel->getBoundingBox().size.width*.9605, mainPanel->getBoundingBox().size.height * 0.9));
    scrollview->setSize(Size(mainPanel->getBoundingBox().size.width*.9605, mainPanel->getBoundingBox().size.height * 0.9));
    scrollview->setPosition(Vec2(mainPanel->getBoundingBox().getMidX(), mainPanel->getBoundingBox().getMidY() + visibleSize.height * 0.0725));
    scrollview->setAnchorPoint(Point(0.49675,0.5));//475
    scrollview->setDirection(cocos2d::ui::ScrollView::Direction::HORIZONTAL);
    scrollview->setInertiaScrollEnabled(true);
    float innerWidth = mainPanel->getBoundingBox().size.width * 3;
    float innerHeight = mainPanel->getBoundingBox().size.height * 0.9;
    scrollview->setInnerContainerSize(Size(innerWidth,innerHeight));
    
    for(int x = 0; x < 10; x++){
        auto seperator1 = MainMenu::CreateButton("HSSeperator.png", Vec2(.009,0.17),Vec2(0,0));
        seperator1->setPosition(Vec2(15 + (x * 550),0));
        seperator1->setAnchorPoint(Vec2(0.0,0));
        seperator1->setScale(visibleSize.height / 960);
        scrollview->addChild(seperator1,1);
        for(int y = 0; y < 4; y++){
            float bottom = y * 88 - 14;
            float left = x * 550 + 62;
            auto userNameTemp = MainMenu::CreateLabel("PlayerName", Vec2(0,1.0-0.015), Vec2(0,0));
            userNameTemp->setPosition(left, bottom);
            userNameTemp->setAnchorPoint(Vec2(0.0,0));
            userNameTemp->setScale(0.77);
            auto userPlaceTemp = MainMenu::CreateLabel(std::to_string(x*4 + (4-y)), Vec2(0,1.0-0.015), Vec2(0,0));
            userPlaceTemp->setPosition(left,bottom + 85 + 21);
            userPlaceTemp->setAnchorPoint(Vec2(0,1));
            userPlaceTemp->setColor(Color3B(255,225,107));
            userPlaceTemp->setScale(0.49);
            auto userTimeTemp = MainMenu::CreateLabel("0:00", Vec2(0,1.0-0.015), Vec2(0,0));
            userTimeTemp->setPosition(left + 490, bottom + 50);
            userTimeTemp->setAnchorPoint(Vec2(1,0.5));
            userTimeTemp->setColor(Color3B(53,107,168));
            if(y%2 == 0)
                userTimeTemp->setColor(Color3B(170,201,228));
            scrollview->addChild(userNameTemp,1);
            scrollview->addChild(userPlaceTemp,1);
            scrollview->addChild(userTimeTemp,1);
        
        }
    }
    
    auto titleBackground = MainMenu::CreateButton("HSTitlePanel.png", Vec2(0,0), Vec2(0,0));
    titleBackground->setAnchorPoint(Vec2(0.5,1.0));
    titleBackground->setPosition(Vec2(background->getBoundingBox().getMidX(), background->getBoundingBox().getMaxY()));
    menuItems.pushBack(titleBackground);
    
    levelName = MainMenu::CreateLabel("Leve Name", Vec2(0,1.0-0.015), Vec2(0,0));
    levelName->setPosition(titleBackground->getBoundingBox().getMidX(), titleBackground->getBoundingBox().getMidY());
    levelName->setAnchorPoint(Vec2(0.5,0.5));
    
    userName = MainMenu::CreateLabel("PlayerName", Vec2(0,1.0-0.015), Vec2(0,0));
    userName->setPosition(userPanel->getBoundingBox().getMinX() + (visibleSize.width * 0.05), userPanel->getBoundingBox().getMinY());
    userName->setAnchorPoint(Vec2(0.0,0));
    userName->setScale(0.83);
    
    userPlace = MainMenu::CreateLabel("130,023", Vec2(0,1.0-0.015), Vec2(0,0));
    userPlace->setPosition(userPanel->getBoundingBox().getMinX() + (visibleSize.width * 0.05), userPanel->getBoundingBox().getMaxY());
    userPlace->setAnchorPoint(Vec2(0,1));
    userPlace->setColor(Color3B(255,225,107));
    userPlace->setScale(0.49);
    
    userTime = MainMenu::CreateLabel("0:00", Vec2(0,1.0-0.015), Vec2(0,0));
    userTime->setPosition(userPanel->getBoundingBox().getMaxX() - 15, userPanel->getBoundingBox().getMaxY());
    userTime->setAnchorPoint(Vec2(1,1));
    userTime->setColor(Color3B(53,107,168));
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(levelName,1);
    this->addChild(userName,1);
    this->addChild(userPlace,1);
    this->addChild(userTime,1);
    this->addChild(slider2,1);
    this->addChild(scrollview,1);
    
    //-- Set Up Newtork Stuff --//
    //NDKHelper::addSelector("LogIn", "userCheckCallback", CC_CALLBACK_2(MainMenu::userCheckCallback, this), this);
    //--------------------------//
    
    return true;
}
void Highscores::playButtonCallback(Ref*){
    goToPlay();
};
void Highscores::favoriteButtonCallback(Ref*){

};
void Highscores::facebookButtonCallback(Ref*){

};
void Highscores::homeButtonCallback(Ref*){
    goToLevelSelect();
};
void Highscores::sliderCallback(Ref*){

};
void Highscores::goToLevelSelect(){
    if(LevelSelect::myScene == NULL){
        auto scene = LevelSelect::createScene();
        Director::getInstance()->pushScene(scene);
        LevelSelect::Instance->Refresh();
    }
    else{
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(transition);
        LevelSelect::Instance->Refresh();
    }
};
void Highscores::goToPlay(){
    if(currentLevel != NULL){
        if(Game::myScene == NULL){
            auto scene = Game::createScene();
            auto transition = TransitionFade::create(MainMenu::transitionTime, Game::myScene);
            Director::getInstance()->pushScene(transition);
            Game::Instance->Reset(currentLevel);
        }
        else{
            auto transition = TransitionFade::create(MainMenu::transitionTime, Game::myScene);
            Director::getInstance()->pushScene(transition);
            Game::Instance->Reset(currentLevel);
        }
    }
}
void Highscores::SetLevel(Level* lvl){
    currentLevel = lvl;
}
/*void Highscores::registerCallback(Node* sender, cocos2d::Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        ValueMap valueMap = data.asValueMap();
        std::string success = valueMap["success"].asString();
        std::string responce = valueMap["responce"].asString();
        if(success.compare("true") == 0){
            //if registering is successfull then logInCallback will be called not registerCallback.
        }
        else{
            printf("Failed To Register: %s", responce.c_str());
        }
    }
};*/
bool Highscores::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void Highscores::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
}
void Highscores::menuCloseCallback(Ref* pSender)
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
