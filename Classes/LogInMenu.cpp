//
//  LogInMenu.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 8/7/14.
//
//

#include "LogInMenu.h"
#include "Game.h"
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
    
    /*
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
     */
    
    std::map<std::string, SEL_MenuHandler> callbacks;
    callbacks["RegisterButton"] = menu_selector(LogInMenu::registerButtonCallback);
    callbacks["LogInBackground"] = menu_selector(LogInMenu::logInButtonCallback);
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("login", this, callbacks, &menuItems, this);
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    Size editBoxSize = Size(elements["UsernamePanel"]->getBoundingBox().size.width * 0.5, elements["UsernamePanel"]->getBoundingBox().size.height * 0.75);
    Scale9Sprite* passwordBoxBG = Scale9Sprite::create("Slice_9_Inlay.png");
    passwordBoxBG->setCapInsets(Rect(18,32,89,70));
    passwordBoxBG->setContentSize(editBoxSize);
    passwordBoxBG->setAnchorPoint(Vec2(0.1,0));
    passwordBox = EditBox::create(editBoxSize, passwordBoxBG);
    passwordBox->setFontColor(Color3B::BLACK);
    passwordBox->setPlaceholderFontColor(Color3B::BLACK);
    passwordBox->setPosition(Point(elements["PasswordPanel"]->getBoundingBox().getMaxX() * 0.985, elements["PasswordPanel"]->getBoundingBox().getMidY()));
    passwordBox->setAnchorPoint(Vec2(1,0.5));
    passwordBox->setFontSize(90);
    passwordBox->setMaxLength(18);
    passwordBox->setPlaceHolder(" Password");
    passwordBox->setFontName("Marker Felt.ttf");
    passwordBox->setInputFlag(EditBox::InputFlag::PASSWORD);
    this->addChild(passwordBox,1);
    
    Scale9Sprite* usernameBoxBG = Scale9Sprite::create("Slice_9_Inlay.png");
    usernameBoxBG->setCapInsets(Rect(18,32,89,70));
    usernameBoxBG->setContentSize(editBoxSize);
    usernameBox = EditBox::create(editBoxSize, usernameBoxBG);
    usernameBox->setPosition(Point(elements["UsernamePanel"]->getBoundingBox().getMaxX() * 0.985, elements["UsernamePanel"]->getBoundingBox().getMidY()));
    usernameBox->setAnchorPoint(Vec2(1,0.5));
    usernameBox->setFontSize(90);
    usernameBox->setMaxLength(18);
    usernameBox->setPlaceHolder(" Username");
    usernameBox->setFontName("Marker Felt.ttf");
    usernameBox->setPlaceholderFontColor(Color3B::BLACK);
    usernameBox->setFontColor(Color3B::BLACK);
    this->addChild(usernameBox,1);
    
    auto logInLabel = MainMenu::CreateLabel("Log In", 1);
    logInLabel->setGlobalZOrder(0);
    logInLabel->setPosition(Vec2(elements["LogInBackground"]->getBoundingBox().getMidX(), elements["LogInBackground"]->getBoundingBox().getMidY()));
    this->addChild(logInLabel, 1);
    
    auto registerLabel = MainMenu::CreateLabel("Register", 1);
    registerLabel->setGlobalZOrder(0);
    registerLabel->setPosition(Vec2(elements["RegisterButton"]->getBoundingBox().getMidX(), elements["RegisterButton"]->getBoundingBox().getMidY()));
    this->addChild(registerLabel, 1);
    
    auto titleLabel = MainMenu::CreateLabel("This'll Only Take a Second", 1);
    titleLabel->setGlobalZOrder(0);
    titleLabel->setPosition(Vec2(elements["TitlePanel"]->getBoundingBox().getMidX(), elements["TitlePanel"]->getBoundingBox().getMidY()));
    this->addChild(titleLabel, 1);
    
    auto passwordLabel = MainMenu::CreateLabel("Password:", 1);
    passwordLabel->setGlobalZOrder(0);
    passwordLabel->setPosition(Vec2(elements["PasswordPanel"]->getBoundingBox().getMinX() * 1.25, elements["PasswordPanel"]->getBoundingBox().getMidY()));
    passwordLabel->setAnchorPoint(Vec2(0,0.5));
    this->addChild(passwordLabel, 1);
    
    auto usernameLabel = MainMenu::CreateLabel("Username:", 1);
    usernameLabel->setGlobalZOrder(0);
    usernameLabel->setPosition(Vec2(elements["UsernamePanel"]->getBoundingBox().getMinX() * 1.25, elements["UsernamePanel"]->getBoundingBox().getMidY()));
    usernameLabel->setAnchorPoint(Vec2(0,0.5));
    this->addChild(usernameLabel, 1);
    
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
