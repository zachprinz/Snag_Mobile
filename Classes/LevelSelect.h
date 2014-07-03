//
//  LevelSelect.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/3/14.
//
//

#ifndef __Snag__LevelSelect__
#define __Snag__LevelSelect__

#include <iostream>
#include "Board.h"
#include "cocos2d.h"
#include "LevelMenuItem.h"

#define LEVELS_ONLINE 1
#define LEVELS_LOCAL 2
#define LEVELS_MY 3

USING_NS_CC;


class LevelSelect : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    CREATE_FUNC(LevelSelect);
    
    void editCallback(Ref*);
    void playCallback(Ref*);
    void highscoresCallback(Ref*);
    void scrollUpCallback(Ref*);
    void scrollDownCallback(Ref*);
    void localCallback(Ref*);
    void myLevelsCallback(Ref*);
    void onlineCallback(Ref*);
    void homeCallback(Ref*);
    
    std::vector<LevelMenuItem*> levels;
    void AddLevel(LevelMenuItem*);
    void LoadLevels();
    
    static Scene* myScene;
    static Vec2 screenScale;
    
    Sprite* myLevelsBackground;
    Sprite* onlineLevelsBackground;
    Sprite* localLevelsBackground;
    int currentLevelSet;
private:
    
};

#endif /* defined(__Snag__LevelSelect__) */