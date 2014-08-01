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
#include "CocosGUI.h"
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
    void playCallback(Object*);
    void highscoresCallback(Ref*);
    void scrollLeftCallback(Ref*);
    void scrollRightCallback(Ref*);
    void risingCallback(Ref*);
    void customCallback(Ref*);
    void socialCallback(Ref*);
    void favoritedCallback(Ref*);
    void newLevelCallback(Ref*);
    void facebookCallback(Ref*);
    
    float tabHeightSelected;
    float tabHeight;
    
    std::vector<LevelMenuItem*> levels;
    void AddLevel(LevelMenuItem*);
    void LoadLevels();
    
    static Scene* myScene;
    static Vec2 screenScale;
    
    MenuItemImage* levelsBackground;
    int currentLevelSet;
    
    void Refresh();
    
    static LevelSelect* Instance;
    void SetLevelSetButtons(int newSet);
private:
    MenuItemImage* customLevels;
    MenuItemImage* socialLevels;
    MenuItemImage* risingLevels;
    MenuItemImage* favoritedLevels;
};

#endif /* defined(__Snag__LevelSelect__) */
