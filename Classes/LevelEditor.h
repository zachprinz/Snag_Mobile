//
//  LevelEditor.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#ifndef __Snag__LevelEditor__
#define __Snag__LevelEditor__

#include <iostream>
#include "Game.h"
#include "cocos2d.h"
#include "MapObject.h"
#include "cocos-ext.h"
#include "PopUp.h"
#include "Preview.h"

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5
#define ERASE 6
#define NO_TOOL 7
#define MOVE 8
#define GOAL 9
#define TILE_SIZE 40

using namespace cocos2d::extension;

class LevelEditor : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(LevelEditor);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
    void onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
    void onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    
    void SpikeWallSelectCallback(Ref*);
    void WallSelectCallback(Ref*);
    void SpawnerSelectCallback(Ref*);
    void HookSelectCallback(Ref*);
    void EraseSelectCallback(Ref*);
    void homeButtonCallback(Ref*);
    void trashButtonCallback(Ref*);
    void saveButtonCallback(Ref*);
    void moveButtonCallback(Ref*);
    void playButtonCallback(Ref*);
    void selectedButtonCallback(Ref*);
    void saveAcceptCallback(Ref*);
    void saveDeclineCallback(Ref*);
    void GoalSelectCallback(Ref*);
    void closePopUpCallback(Ref*);
    
    MenuItemImage* homeSelectButton;
    MenuItemImage* hookSelectButton;
    MenuItemImage* spikeWallSelectButton;
    MenuItemImage* spawnerSelectButton;
    MenuItemImage* eraseSelectButton;
    MenuItemImage* wallSelectButton;
    MenuItemImage* playSelectButton;
    MenuItemImage* moveSelectButton;
    MenuItemImage* goalSelectButton;
    MenuItemImage* saveAcceptButton;
    MenuItemImage* saveDeclineButton;
    MenuItemImage* trashSelectButton;
    MenuItemImage* saveSelectButton;
    MenuItemImage* currentSelection;
    MenuItemImage* shade;
    MenuItemImage* selectedSprite;
    Label* selectedLabel;
    Label* notice;
    Menu* menu;
    Menu* saveMenu;
    EditBox* nameBox;
    
    virtual void editBoxEditingDidBegin(EditBox* editBox);
    virtual void editBoxEditingDidEnd(EditBox* editBox);
    virtual void editBoxTextChanged(EditBox* editBox, std::string &text);
    virtual void editBoxReturn(EditBox * editBox);

    Level* currentLevel;
    void SetLevel(Level*);
    
    static Scene* myScene;
    bool isTouched;
    Vec2 touchStart;
    Vec2 touchCurrent;
    int currentTool;
    std::string name;
    bool noticeUp;
    void DisableSpawner();
    void EnableSpawner();
    void Erase(Vec2);
    bool saveDialog;
    static LevelEditor* Instance;
    void SetToolPos();
    void ResetToolPos();
    float saveButtonsOnY;
    float saveButtonsOffY;
    void ResetSaveDialog();
    void Export();
    void Clear();
    
    PopUp* savePopUp;
    PopUp* spawnerPopUp;
    PopUp* hookPopUp;
private:
    int numberOfTouches;
    std::vector<Touch*> currentTouches;
    std::map<int,Entity*> entities;
    Preview* preview;
    Scale9Sprite* currentSprite;
};

#endif /* defined(__Snag__LevelEditor__) */
