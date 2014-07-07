//
//  LevelInfo.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/7/14.
//
//

#ifndef __Snag__LevelInfo__
#define __Snag__LevelInfo__

#include <iostream>
#include "cocos2d.h"
#include "Board.h"
class LevelInfo : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    
    // implement the "static create()" method manually
    CREATE_FUNC(LevelInfo);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void homeButtonCallback(Ref*);
    void favoriteButtonCallback(Ref*);
    void highscoresButtonCallback(Ref*);
    void editButtonCallback(Ref*);
    void downloadButtonCallback(Ref*);
    void playButtonCallback(Ref*);
    
    Label* title;
    Label* author;
    
    static Scene* myScene;
private:
    
};

#endif /* defined(__Snag__LevelInfo__) */
