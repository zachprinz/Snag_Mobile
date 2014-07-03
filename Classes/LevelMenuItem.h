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
    cocos2d::MenuItemFont* name;
    void SetOrder(int);
    
private:
};

#endif /* defined(__Snag__LevelMenuItem__) */
