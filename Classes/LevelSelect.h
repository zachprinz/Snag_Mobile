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
#include "Game.h"
#include "cocos2d.h"
#include "CocosGUI.h"
#include "LevelMenuItem.h"
#include "PopUp.h"
#include "Preview.h"

#define LEVELS_CUSTOM 0
#define LEVELS_SOCIAL 1
#define LEVELS_FAVORITED 2
#define LEVELS_RISING 3

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
    void scrollLeftCallback(Ref*);
    void scrollRightCallback(Ref*);
    void risingCallback(Ref*);
    void customCallback(Ref*);
    void socialCallback(Ref*);
    void favoritedCallback(Ref*);
    void newLevelCallback(Ref*);
    void deleteCallback(Ref*);
    void favoriteCallback(Ref*);
    void selectCallback(Ref*);
    void uploadCallback(Ref*);
    void deleteAcceptCallback(Ref*);
    void deleteDeclineCallback(Ref*);

    float tabHeightSelected;
    float tabHeight;
    
    std::vector<LevelMenuItem*> levels;
    void LoadLevels();
    
    static Scene* myScene;
    static Vec2 screenScale;
    
    MenuItemImage* levelsBackground;
    int currentLevelSet;
    
    void Refresh();
    
    static LevelSelect* Instance;
    void SetLevelSetButtons();
    void SetLevelSet(int);
    Level* selectedLevel;
    int page;
    PopUp* deletePopUp;
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    Vec2 touchStart;
    Vec2 touchCurrent;
    bool isDragging;
private:
    Preview* preview;
    bool goToEdit;
    void showLoading();
    void goToLevelEditor();
    void SetPreview();
    MenuItemImage* currentLevelsTab;
    MenuItemImage* customLevels;
    MenuItemImage* socialLevels;
    MenuItemImage* risingLevels;
    MenuItemImage* favoritedLevels;
    MenuItemImage* highscoresButton;
    MenuItemImage* uploadButton;
    MenuItemImage* previewWindow;
    MenuItemImage* hand;
    Sprite* loading;
    Label* previewTitle;
    Label* previewAuthor;
    void FetchSocialLevels();
    void FetchCustomLevels();
    void FetchRisingLevels();
    void FetchFavoritedLevels();
    void fetchSocialCallback(Node* sender, Value data);
    void fetchFavoritedCallback(Node* sender, Value data);
    void fetchRisingCallback(Node* sender, Value data);
    void fetchCustomCallback(Node* sender, Value data);
    void doneFetching(Node* sender, Value data);
    void favCallback(Node* sender, Value data);
    void levelDeletedCallback(Node* sender, Value data);
    void newLevelResponce(Node* sender, Value data);
};

#endif /* defined(__Snag__LevelSelect__) */
