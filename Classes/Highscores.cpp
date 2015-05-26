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
    
    std::map<std::string, SEL_MenuHandler> callbacks;
    callbacks["Facebook"] = menu_selector(Highscores::facebookButtonCallback);
    callbacks["Play"] = menu_selector(Highscores::playButtonCallback);
    callbacks["Home"] = menu_selector(Highscores::homeButtonCallback);
    callbacks["Star"] = menu_selector(Highscores::favoriteButtonCallback);
    callbacks["ScrollerHandle"] = menu_selector(Highscores::sliderCallback);
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("highscores", this, callbacks, &menuItems, this);
    
    scrollview = ui::ScrollView::create();
    scrollview->setContentSize(Size(elements["Inlay"]->getBoundingBox().size.width, elements["Inlay"]->getBoundingBox().size.height * 0.9));
    scrollview->setSize(Size(elements["Inlay"]->getBoundingBox().size.width, elements["Inlay"]->getBoundingBox().size.height));
    scrollview->setPosition(Vec2(elements["Inlay"]->getBoundingBox().getMidX(), elements["Inlay"]->getBoundingBox().getMidY()));
    scrollview->setAnchorPoint(Point(0.5,0.5));
    scrollview->setDirection(ui::ScrollView::Direction::HORIZONTAL);
    scrollview->setInertiaScrollEnabled(true);
    float innerWidth = elements["Inlay"]->getBoundingBox().size.width * 3;
    float innerHeight = elements["Inlay"]->getBoundingBox().size.height;
    scrollview->setInnerContainerSize(Size(innerWidth,innerHeight));
    elements["Seperator"]->setVisible(false);
    if(MainMenu::aspectRatio == 0){
        elements["Stripes"]->setScaleY(elements["Stripes"]->getScaleY() * 0.98);
        elements["Stripes"]->setPositionY(scrollview->getBoundingBox().getMinY() + scrollview->getBoundingBox().size.height * 0.02);
        elements["Stripes"]->setAnchorPoint(Vec2(0,0));
    }
    for(int x = 0; x < 10; x++){
        auto seperator1 = MainMenu::CreateButton("highscores", "Seperator.png");
        seperator1->setPosition(Vec2(x * 750 * (visibleSize.width / 1920.0),0));
        seperator1->setAnchorPoint(Vec2(0,0));
        scrollview->addChild(seperator1,1);
        for(int y = 0; y < 4; y++){
            float bottom = y * (0.25 * elements["Inlay"]->getBoundingBox().size.height);
            float left = seperator1->getBoundingBox().getMaxX() + seperator1->getBoundingBox().size.width * 0.1;
            auto userNameTemp = MainMenu::CreateLabel("PlayerName", 1);
            userNameTemp->setPosition(left, bottom);
            userNameTemp->setAnchorPoint(Vec2(0.0,0));
            auto userPlaceTemp = MainMenu::CreateLabel(std::to_string(x*4 + (4-y)), 0);
            userPlaceTemp->setPosition(left, userNameTemp->getBoundingBox().getMaxY() - (userNameTemp->getBoundingBox().size.height * 0.3));
            userPlaceTemp->setAnchorPoint(Vec2(0,0));
            userPlaceTemp->setColor(Color3B(255,225,107));
            auto userTimeTemp = MainMenu::CreateLabel("0:00", 1);
            userTimeTemp->setPosition(left + 750 * (visibleSize.width / 1920.0) - seperator1->getBoundingBox().size.width * 1.1, bottom + 50);
            userTimeTemp->setAnchorPoint(Vec2(1,0.5));
            userTimeTemp->setColor(Color3B(53,107,168));
            if(y%2 == 0)
                userTimeTemp->setColor(Color3B(170,201,228));
            scrollview->addChild(userNameTemp,1);
            scrollview->addChild(userPlaceTemp,1);
            scrollview->addChild(userTimeTemp,1);
        }
    }
    
    levelName = MainMenu::CreateLabel("Leve Name", 2);
    levelName->setPosition(elements["TitlePanel"]->getBoundingBox().getMidX(), elements["TitlePanel"]->getBoundingBox().getMidY());
    levelName->setAnchorPoint(Vec2(0.5,0.5));
    
    userName = MainMenu::CreateLabel("PlayerName", 1);
    userName->setPosition(elements["UserPanel"]->getBoundingBox().getMinX() + (visibleSize.width * 0.05), elements["UserPanel"]->getBoundingBox().getMinY());
    userName->setAnchorPoint(Vec2(0.0,0));
    
    userPlace = MainMenu::CreateLabel("130,023", 0);
    userPlace->setPosition(userName->getBoundingBox().getMinX(), userName->getBoundingBox().getMaxY() - (userName->getBoundingBox().size.height * 0.3));
    userPlace->setAnchorPoint(Vec2(0,0));
    userPlace->setColor(Color3B(255,225,107));
    
    userTime = MainMenu::CreateLabel("0:00", 1);
    userTime->setPosition(elements["UserStripe"]->getBoundingBox().getMaxX() - userTime->getBoundingBox().size.width * 0.1, elements["UserStripe"]->getBoundingBox().getMidY());
    userTime->setAnchorPoint(Vec2(1,0.5));
    userTime->setColor(Color3B(53,107,168));
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(levelName,1);
    this->addChild(userName,1);
    this->addChild(userPlace,1);
    this->addChild(userTime,1);
    //this->addChild(slider2,1);
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
