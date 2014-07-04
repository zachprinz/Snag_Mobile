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

using namespace cocos2d;

class LevelMenuItem{
public:
    LevelMenuItem(std::string, Size visibleSize,int x,  int y);
    cocos2d::MenuItemImage* edit;
    cocos2d::MenuItemImage* play;
    cocos2d::MenuItemImage* highscores;
    cocos2d::Sprite* background;
    cocos2d::Label* name;
    void SetOrder(int);
    void SetName(const char*);
    void SetPath(const char*);
    const char* myName;
    const char* myPath;
    void SetEnabled(bool);
private:
};

#endif /* defined(__Snag__LevelMenuItem__) */
