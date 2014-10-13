//
//  PopUp.h
//  Snag
//
//  Created by Zachary Prinzbach on 8/11/14.
//
//

#ifndef __Snag__PopUp__
#define __Snag__PopUp__

#include <iostream>
#include "cocos2d.h"
#include "MainMenu.h"
#if(CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
	#include "extensions/cocos-ext.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	#include "cocos-ext.h"
#endif

using namespace cocos2d;
using namespace cocos2d::extension;

class PopUp{
public:
    static PopUp* Instance;
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback);
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler);
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler, bool x);
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler, SEL_MenuHandler, int);
    cocos2d::MenuItemImage* acceptButton;
    cocos2d::MenuItemImage* declineButton;
    cocos2d::MenuItemImage* thirdButton;
    cocos2d::MenuItemImage* background;
    cocos2d::MenuItemImage* shade;
    cocos2d::Label* name;
    cocos2d::Label* text;
    cocos2d::Label* option1;
    cocos2d::Label* option2;
    cocos2d::Label* option3;
    EditBox* nameBox;
    std::string myName;
    std::string myText;
    void Show();
    void Close();
    void Add(Layer*);
    std::string GetText();
    Menu* menu;
    void Remove(Layer*);
    bool visible;
private:
};

#endif /* defined(__Snag__PopUp__) */
