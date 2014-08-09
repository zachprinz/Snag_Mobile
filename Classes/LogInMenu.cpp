//
//  LogInMenu.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 8/7/14.
//
//

#include "LogInMenu.h"
#include "Board.h"
#include "HelloWorldScene.h"
#include "MainMenu.h"
#include "LevelSelect.h"
#include "NDKHelper/NDKHelper.h"

USING_NS_CC;
#define MAX_SCALE 1.5;
#define NATIVE_WIDTH 1704;
#define NATIVE_HEIGHT 960;

Scene* LogInMenu::myScene;
LogInMenu* LogInMenu::Instance;

Scene* LogInMenu::createScene(){
    auto scene = Scene::create();
    auto layer = LogInMenu::create();
    myScene = scene;
    Instance = layer;
    scene->addChild(layer);
    return scene;
}

bool LogInMenu::init()
{
    if(!Layer::init()){
        return false;
    }
    Instance = this;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto background = MainMenu::CreateButton("MMBackground.png", Vec2(0,1), Vec2(0,0));
    background->setPositionZ(-2);
    menuItems.pushBack(background);
    
    auto usernameBackground = MainMenu::CreateButton("LIUsername.png", Vec2(0,0),Vec2(0,0));
    usernameBackground->setAnchorPoint(Vec2(0.5,usernameBackground->getAnchorPoint().y));
    usernameBackground->setPosition(Vec2(background->getBoundingBox().getMidX(), (1.0-0.2)*MainMenu::screenSize.y));
    menuItems.pushBack(usernameBackground);
    
    auto passwordBackground = MainMenu::CreateButton("LIPassword.png", Vec2(0,0),Vec2(0,0));
    passwordBackground->setAnchorPoint(Vec2(0.5,passwordBackground->getAnchorPoint().y));
    passwordBackground->setPosition(Vec2(background->getBoundingBox().getMidX(), (1.0-0.47)*MainMenu::screenSize.y));
    menuItems.pushBack(passwordBackground);
    
    auto logInButton = MainMenu::CreateButton("LILogIn.png", this, menu_selector(LogInMenu::logInButtonCallback), Vec2(1.0-0.24, 1.0-0.75), Vec2(0,0));
    logInButton->setAnchorPoint(Vec2(0.5,1));
    menuItems.pushBack(logInButton);
    
    auto registerButton = MainMenu::CreateButton("LIRegister.png", this, menu_selector(LogInMenu::registerButtonCallback), Vec2(0.24, 1.0-0.75), Vec2(0,0));
    registerButton->setAnchorPoint(Vec2(0.5,1));
    menuItems.pushBack(registerButton);
    
    auto title = MainMenu::CreateButton("LITitle.png", Vec2(0,0), Vec2(0,0));
    title->setAnchorPoint(Vec2(0.5,title->getAnchorPoint().y));
    title->setPosition(Vec2(background->getBoundingBox().getMidX(), (1.0-0.00)*MainMenu::screenSize.y));
    menuItems.pushBack(title);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    Size editBoxSize = Size(750*MainMenu::screenScale.x, 100 * MainMenu::screenScale.y);
    Scale9Sprite* passwordBoxBG = Scale9Sprite::create("line.png");
    passwordBoxBG->setContentSize(editBoxSize);
    passwordBox = EditBox::create(Size(350,50), passwordBoxBG);
    passwordBox->setPosition(Point(visibleSize.width / 2.0 + 175, visibleSize.height / 2.0 - 60));
    passwordBox->setFontSize(30);
    passwordBox->setMaxLength(18);
    passwordBox->setPlaceHolder("password");
    passwordBox->setFontColor(Color3B::WHITE);
    this->addChild(passwordBox,1);
    
    Scale9Sprite* usernameBoxBG = Scale9Sprite::create("line.png");
    usernameBoxBG->setContentSize(editBoxSize);
    usernameBox = EditBox::create(Size(350,50), usernameBoxBG);
    usernameBox->setPosition(Point(visibleSize.width / 2.0 + 175, visibleSize.height / 2.0 + 115));
    usernameBox->setFontSize(30);
    usernameBox->setMaxLength(18);
    usernameBox->setPlaceHolder("username");
    usernameBox->setFontColor(Color3B::WHITE);
    this->addChild(usernameBox,1);
    
    //-- Set Up Newtork Stuff --//
    //NDKHelper::addSelector("LogIn", "userCheckCallback", CC_CALLBACK_2(MainMenu::userCheckCallback, this), this);
    //--------------------------//
    
    return true;
}

void LogInMenu::logInButtonCallback(Ref* pSender){
    std::string username = usernameBox->getText();
    std::string password = passwordBox->getText();
    ValueMap valueMap;
    valueMap["username"] = username;
    valueMap["password"] = password;
    Value parameters = Value(valueMap);
    sendMessageWithParams("logIn", parameters);
    
    goToLevelSelect();
}
void LogInMenu::registerButtonCallback(Ref* pSender){
    std::string username = usernameBox->getText();
    std::string password = passwordBox->getText();
    ValueMap valueMap;
    valueMap["username"] = username;
    valueMap["password"] = password;
    Value parameters = Value(valueMap);
    sendMessageWithParams("register", parameters);
    
    goToLevelSelect();
}
void LogInMenu::goToLevelSelect(){
    if(LevelSelect::myScene == NULL){
        auto scene = LevelSelect::createScene();
        Director::getInstance()->pushScene(scene);
        LevelSelect::Instance->Refresh();
    }
    else{
        Director::getInstance()->pushScene(LevelSelect::myScene);
        LevelSelect::Instance->Refresh();
    }
}
void LogInMenu::registerCallback(Node* sender, cocos2d::Value data){
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
};
void LogInMenu::logInCallback(Node* sender, cocos2d::Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        ValueMap valueMap = data.asValueMap();
        std::string success = valueMap["success"].asString();
        std::string responce = valueMap["responce"].asString();
        if(success.compare("true") == 0){
            printf("Logged In Successfully!");
            goToLevelSelect();
        }
        else{
            printf("Failed Log In: %s", responce.c_str());
        }
    }
};
bool LogInMenu::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void LogInMenu::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
}
void LogInMenu::menuCloseCallback(Ref* pSender)
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
