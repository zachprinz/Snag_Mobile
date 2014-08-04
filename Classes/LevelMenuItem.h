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

using namespace cocos2d;

class LevelMenuItem{
public:
    LevelMenuItem(std::string);
    cocos2d::MenuItemImage* favorited;
    cocos2d::MenuItemImage* background;
    cocos2d::Label* name;
    void SetOrder(int);
    void SetLevel(Level*, int page);
    std::string myName;
    std::string myPath;
    void SetEnabled(bool);
    void SetTag(int);
    Menu* menu;
    Level* level;
    static int count;
private:
};

#endif /* defined(__Snag__LevelMenuItem__) */
