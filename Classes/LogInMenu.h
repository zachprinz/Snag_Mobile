//
//  LogInMenu.h
//  Snag
//
//  Created by Zachary Prinzbach on 8/7/14.
//
//

#ifndef __Snag__LogInMenu__
#define __Snag__LogInMenu__

#include <iostream>
#include "cocos2d.h"
#include <map>
#include "cocos2d.h"
#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class LogInMenu : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(LogInMenu);
    std::map<std::string, cocos2d::MenuItemImage*> elements;

    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void logInButtonCallback(Ref*);
    void registerButtonCallback(Ref*);
    
    static cocos2d::Scene* myScene;
    static LogInMenu* Instance;
    
    ui::EditBox* usernameBox;
    ui::EditBox* passwordBox;
    
    void registerCallback(Node* sender, cocos2d::Value data);
    void logInCallback(Node* sender, cocos2d::Value data);
    
    void goToLevelSelect();
private:
    
};
#endif 

/* defined(__Snag__LogInMenu__) */
