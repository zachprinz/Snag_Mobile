//
//  LevelMenuItem.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/3/14.
//
//

#ifndef __Snag__LevelMenuItem__
#define __Snag__LevelMenuItem__

#include <iostream>
#include "cocos2d.h"
#include "Level.h"
#include "../cocos2d/cocos/ui/UIScrollView.h"
#include "ui/CocosGUI.h"

using namespace cocos2d;

class LevelMenuItem{
public:
    LevelMenuItem(int position, Layer* layer, float center, float inlayHeight);
    LevelMenuItem(int position, ui::ScrollView* layer, float center, float inlayHeight);
    cocos2d::MenuItemImage* favorited;
    cocos2d::MenuItemImage* background;
    cocos2d::Label* name;
    cocos2d::Label* favorites;
    cocos2d::Label* number;
    void SetOrder(int);
    void SetLevel(Level*, int page);
    std::string myName;
    std::string myPath;
    void SetEnabled(bool);
    void SetTag(int);
    Menu* menu;
    Level* level;
    static int count;
    void SetFavorited(bool);
private:
};

#endif /* defined(__Snag__LevelMenuItem__) */
