//
//  MainMenu.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#ifndef __Snag__MainMenu__
#define __Snag__MainMenu__

#include <iostream>
#include "Board.h"
#include "cocos2d.h"

class MainMenu : public cocos2d::Layer
{
public:
    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::Scene* createScene();
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainMenu);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void playButtonCallback(Ref*);
    void levelCreatorButtonCallback(Ref*);
    
    static Scene* myScene;
    static Vec2 screenScale;
private:
    
};

#endif /* defined(__Snag__MainMenu__) */
