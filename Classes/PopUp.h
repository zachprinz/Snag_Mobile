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
#include "cocos-ext.h"

using namespace cocos2d;
using namespace cocos2d::extension;

class PopUp{
public:
    static PopUp* Instance;
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback);
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler);
    PopUp(std::string title, std::string text, Ref* ref, SEL_MenuHandler callback, SEL_MenuHandler, bool x);
    cocos2d::MenuItemImage* acceptButton;
    cocos2d::MenuItemImage* declineButton;
    cocos2d::MenuItemImage* background;
    cocos2d::MenuItemImage* shade;
    cocos2d::Label* name;
    cocos2d::Label* text;
    EditBox* nameBox;
    std::string myName;
    std::string myText;
    void Show();
    void Close();
    void Add(Layer*);
    std::string GetText();
    Menu* menu;
    void Remove(Layer*);
private:
};

#endif /* defined(__Snag__PopUp__) */
