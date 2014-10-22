//
//  MainMenu.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#ifndef __Snag__MainMenu__
#define __Snag__MainMenu__

#define AR_3X2
#define AR_4X3
#define AR_16X9

#include <iostream>
#include "Game.h"
#include "cocos2d.h"
#include <map>
#include "string.h"

class MainMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void playButtonCallback(Ref*);
    void emptyCallback(Ref*){};
    std::map<std::string, MenuItemImage*> elements;
    
    static Scene* myScene;
    static MainMenu* Instance;
    static Vec2 screenScale;
    static float minScreenScale;
    static Vec2 screenSize;
    
    static MenuItemImage* CreateButton(std::string imagePath, Ref* ref, SEL_MenuHandler callback, Vec2 pos, Vec2 anchors);
    static MenuItemImage* CreateButton(std::string imagePath, Vec2 pos, Vec2 anchors);
    static MenuItemImage* CreateButton(std::string layer, std::string item, Ref* ref, SEL_MenuHandler callback);
    static MenuItemImage* CreateButton(std::string layer, std::string item);
    static Label* CreateLabel(std::string text, Vec2 pos, Vec2 anchors);
    static Vec2 GetAdjustedScale(Vec2, Vec2);
    static Vec2 GetAdjustedPosition(Vec2, Vec2, Vec2);
    
    static int aspectRatio;
    static std::string ar_extension;
    static float ar_scale;
    static float ar_offset;
    static std::map<std::string, MenuItemImage*> LoadElementMap(std::string xmlPath, Ref* ref, std::map<std::string, SEL_MenuHandler> callbacks, cocos2d::Vector<MenuItem*>* menuItems, cocos2d::Layer*);
    static std::string GetPath(std::string, std::string);

    
    void userCheckCallback(Node* sender, Value data);
    void checkForUser();
    void goToLogIn();
    void goToLevelSelect();
    static float transitionTime;
    Label* notice;
    Menu* menu;
private:
    
};

#endif /* defined(__Snag__MainMenu__) */
