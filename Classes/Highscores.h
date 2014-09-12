//
//  Highscores.h
//  Snag
//
//  Created by Zachary Prinzbach on 9/12/14.
//
//

#ifndef __Snag__Highscores__
#define __Snag__Highscores__

#include <iostream>
#include "cocos2d.h"
#include "cocos-ext.h"
#include "cocos2d.h"
#include "Level.h"
#include "Game.h"
#include "MainMenu.h"
#include "LevelSelect.h"

using namespace cocos2d::extension;

class Highscores : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(Highscores);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    static cocos2d::Scene* myScene;
    static Highscores* Instance;
    
    //void getHighscoresCallback(Node* sender, cocos2d::Value data);
    
    void playButtonCallback(Ref*);
    void favoriteButtonCallback(Ref*);
    void facebookButtonCallback(Ref*);
    void homeButtonCallback(Ref*);
    void sliderCallback(Ref*);
    
    MenuItemImage* slider;
    cocos2d::ui::Slider* slider2;
    Label* levelName;
    Label* userName;
    Label* userPlace;
    Label* userTime;
    
    void goToLevelSelect();
    void goToPlay();
    
    void SetLevel(Level*);
    
    Level* currentLevel;
private:
    
};
#endif 

/* defined(__Snag__Highscores__) */
