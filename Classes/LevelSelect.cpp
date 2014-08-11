//
//  LevelSelect.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/3/14.
//
//

#include "LevelSelect.h"
#include "Game.h"
#include "LevelEditor.h"
#include "MainMenu.h"
#include "LevelMenuItem.h"
#include "LevelInfo.h"
#include "NDKHelper/NDKHelper.h"

USING_NS_CC;

Scene* LevelSelect::myScene;
Vec2 LevelSelect::screenScale;
LevelSelect* LevelSelect::Instance;

Scene* LevelSelect::createScene()
{
    auto scene = Scene::create();
    
    auto layer = LevelSelect::create();
    myScene = scene;
    
    scene->addChild(layer);
    
    return scene;
}

bool LevelSelect::init()
{
    if(!Layer::init()){
        return false;
    }
    Instance = this;
    
    currentLevelSet = LEVELS_RISING;
    page = 0;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    cocos2d::Vector<MenuItem*> menuItems;
    
    risingLevels = MainMenu::CreateButton("LSRisingTab.png", this, menu_selector(LevelSelect::risingCallback), Vec2(0.015,1.0-0.105), Vec2(0,0));
    risingLevels->setGlobalZOrder(0);
    socialLevels = MainMenu::CreateButton("LSSocialTab.png", this, menu_selector(LevelSelect::socialCallback), Vec2(0.151,1.0-0.105), Vec2(0,0));
    socialLevels->setGlobalZOrder(0);
    favoritedLevels = MainMenu::CreateButton("LSFavoritedTab.png", this, menu_selector(LevelSelect::favoritedCallback), Vec2(0.29,1.0-0.105), Vec2(0,0));
    favoritedLevels->setGlobalZOrder(0);
    customLevels = MainMenu::CreateButton("LSCustomTab.png", this, menu_selector(LevelSelect::customCallback), Vec2(0.423,1.0-0.105), Vec2(0,0));
    customLevels->setGlobalZOrder(0);
    auto seperator = MainMenu::CreateButton("LSSeperator.png", Vec2(0.113,1.0-0.822), Vec2(0,0));
    seperator->setGlobalZOrder(0);
    auto leftButton = MainMenu::CreateButton("LSLeft.png", this, menu_selector(LevelSelect::scrollLeftCallback), Vec2(0.016,1.0-0.819), Vec2(0,0));
    leftButton->setGlobalZOrder(0);
    auto rightButton = MainMenu::CreateButton("LSRight.png", this, menu_selector(LevelSelect::scrollRightCallback), Vec2(0.47,1.0-0.819), Vec2(0,0));
    rightButton->setGlobalZOrder(0);
    auto newLevelButton = MainMenu::CreateButton("LSNewLevel.png", this, menu_selector(LevelSelect::newLevelCallback), Vec2(0.114,1.0-0.855), Vec2(0,0));
    newLevelButton->setGlobalZOrder(0);
    auto levelTitle = MainMenu::CreateButton("LSPreviewTitle.png", Vec2(0.585,1.0-0.028), Vec2(0,0));
    levelTitle->setGlobalZOrder(0);
    auto levelAuthor = MainMenu::CreateButton("LSPreviewAuthor.png", Vec2(0.621,1.0-0.163), Vec2(0,0));
    levelAuthor->setGlobalZOrder(0);
    auto previewWindow = MainMenu::CreateButton("LSPreviewPreview.png", Vec2(0.582,1.0-0.303), Vec2(0,0));
    previewWindow->setGlobalZOrder(0);
    auto playButton = MainMenu::CreateButton("LSPlay.png", this, menu_selector(LevelSelect::playCallback), Vec2(0.587,1.0-0.808), Vec2(0,0));
    playButton->setGlobalZOrder(0);
    highscoresButton = MainMenu::CreateButton("LSHighscores.png", this, menu_selector(LevelSelect::highscoresCallback), Vec2(0.69,1.0-0.808), Vec2(0,0));
    highscoresButton->setGlobalZOrder(0);
    uploadButton = MainMenu::CreateButton("DownloadOn.png", this, menu_selector(LevelSelect::uploadCallback), Vec2(0.69,1.0-0.808), Vec2(0,0));
    uploadButton->setVisible(false);
    uploadButton->setGlobalZOrder(0);
    auto infoButton = MainMenu::CreateButton("info.png", this, menu_selector(LevelSelect::infoCallback), Vec2(0.789,1.0-0.808), Vec2(0,0));
    infoButton->setGlobalZOrder(0);
    auto editButton = MainMenu::CreateButton("LSEdit.png", this, menu_selector(LevelSelect::editCallback), Vec2(0.887,1.0-0.808), Vec2(0,0));
    editButton->setGlobalZOrder(0);
    auto selectBackground = MainMenu::CreateButton("LSSelectBackground.png",  Vec2(0.01,1.0-0.206), Vec2(0,0));
    selectBackground->setGlobalZOrder(0);
    auto previewBackground = MainMenu::CreateButton("LSPreviewBackground.png", Vec2(0.574,1.0-0.013), Vec2(0,0));
    previewBackground->setGlobalZOrder(0);
    auto title = MainMenu::CreateButton("LSTitle.png", Vec2(0.144,1.0-0.0), Vec2(0,0));
    title->setGlobalZOrder(0);
    auto background = MainMenu::CreateButton("MMBackground.png", Vec2(0,1.0-0.0), Vec2(0,0));
    if(visibleSize.width <= ((Sprite*)background->getNormalImage())->getTextureRect().size.width && visibleSize.height <= ((Sprite*)background->getNormalImage())->getTextureRect().size.height){
        background->setNormalImage(Sprite::create("MMBackground.png", Rect(0,0,visibleSize.width, visibleSize.height)));
    }
    if(background->getScaleX() < 1.0)
        background->setScaleX(1.0);
    if(background->getScaleY() < 1.0)
        background->setScaleY(1.0);
    background->setGlobalZOrder(0);
    previewTitle = MainMenu::CreateLabel("Select A", Vec2(0,0), Vec2(0,0));
    previewTitle->setGlobalZOrder(0);
    previewTitle->setPosition(Vec2(levelTitle->getBoundingBox().getMidX(), levelTitle->getBoundingBox().getMidY()));
    previewTitle->setVisible(false);
    previewAuthor = MainMenu::CreateLabel("Level", Vec2(0,0), Vec2(0,0));
    previewAuthor->setPosition(Vec2(levelAuthor->getBoundingBox().getMidX(), levelAuthor->getBoundingBox().getMidY()));
    previewAuthor->setGlobalZOrder(0);
    previewAuthor->setVisible(false);
    
    menuItems.pushBack(background);
    menuItems.pushBack(previewBackground);
    menuItems.pushBack(risingLevels);
    menuItems.pushBack(customLevels);
    menuItems.pushBack(socialLevels);
    menuItems.pushBack(favoritedLevels);
    menuItems.pushBack(selectBackground);
    menuItems.pushBack(leftButton);
    menuItems.pushBack(rightButton);
    menuItems.pushBack(seperator);
    menuItems.pushBack(newLevelButton);
    menuItems.pushBack(previewWindow);
    menuItems.pushBack(levelTitle);
    menuItems.pushBack(levelAuthor);
    menuItems.pushBack(infoButton);
    menuItems.pushBack(editButton);
    menuItems.pushBack(playButton);
    menuItems.pushBack(highscoresButton);
    menuItems.pushBack(uploadButton);
    menuItems.pushBack(title);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(previewTitle, 1);
    this->addChild(previewAuthor, 1);

    currentLevelsTab = risingLevels;
    
    for(int x = 0; x < 4; x++){
        LevelMenuItem* lvl = new LevelMenuItem(std::to_string(x) + ") New Map");
        lvl->SetTag(x);
        levels.push_back(lvl);
        this->addChild(lvl->menu,1);
        this->addChild(lvl->name,1);
        this->addChild(lvl->favorites,1);
    }
    
    loading = Sprite::create("Loading.png");
    loading->setScale(0.5,0.5);
    loading->setPosition(levels[1]->background->getBoundingBox().getMinY(), levels[1]->background->getBoundingBox().getMidX());
    loading->setAnchorPoint(Vec2(0.5, 0.5));
    loading->setVisible(false);
    this->addChild(loading,1);
    
    tabHeight = socialLevels->getPosition().y;
    tabHeightSelected = tabHeight - 20;
    currentLevelsTab->setPositionY(tabHeightSelected);
    
    //-- Set Up Newtork Stuff --//
    NDKHelper::addSelector("LevelSelect", "fetchCustomCallback", CC_CALLBACK_2(LevelSelect::fetchCustomCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "fetchSocialCallback", CC_CALLBACK_2(LevelSelect::fetchSocialCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "fetchFavoritedCallback", CC_CALLBACK_2(LevelSelect::fetchFavoritedCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "fetchRisingCallback", CC_CALLBACK_2(LevelSelect::fetchRisingCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "doneFetching", CC_CALLBACK_2(LevelSelect::doneFetching, this), this);
    NDKHelper::addSelector("LevelSelect", "favCallback", CC_CALLBACK_2(LevelSelect::favCallback, this), this);
    //--------------------------//
    
    return true;
}
void LevelSelect::editCallback(Ref*){
    if(selectedLevel != nullptr)
        goToLevelEditor();
}
void LevelSelect::goToLevelEditor(){
    if(LevelEditor::myScene == NULL){
        auto scene = LevelEditor::createScene();
        LevelEditor::Instance->SetLevel(selectedLevel);
        Director::getInstance()->pushScene(scene);
    }
    else{
        LevelEditor::Instance->SetLevel(selectedLevel);
        Director::getInstance()->pushScene(LevelEditor::myScene);
    }
}
void LevelSelect::highscoresCallback(Ref*){

}
void LevelSelect::uploadCallback(Ref*){
    selectedLevel->makePublic();
    if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Public") == 0){
        uploadButton->setVisible(false);
        uploadButton->setEnabled(false);
        highscoresButton->setVisible(true);
        highscoresButton->setEnabled(true);
    }
}
void LevelSelect::scrollRightCallback(Ref*){
    if(Game::levels[currentLevelSet].size() > (page+1)*4){
        page++;
        LoadLevels();
    }
}
void LevelSelect::scrollLeftCallback(Ref*){
    if(page > 0){
        page--;
        LoadLevels();
    }
}
void LevelSelect::newLevelCallback(Ref*){
    sendMessageWithParams("newLevel", Value());
    Refresh();
}
void LevelSelect::infoCallback(Ref*){
    
}
void LevelSelect::favoriteCallback(Ref* ref){
    int tag = ((MenuItemImage*)ref)->getTag();
    if(levels[tag]->level->GetIsFavorited() == false){
        std::string favoritedID = levels[tag]->level->GetID();
        ValueMap valueMap;
        valueMap["id"] = favoritedID;
        Value parameters = Value(valueMap);
        sendMessageWithParams("favorite", parameters);
        tag = ((MenuItemImage*)ref)->getTag();
        selectedLevel = levels[tag]->level;
        levels[tag]->SetFavorited(true);
        SetPreview();
    }
}
void LevelSelect::favCallback(Node* sender, Value data){
    //selectedLevel;
};
void LevelSelect::selectCallback(Ref* ref){
    int tag = ((MenuItemImage*)ref)->getTag();
    selectedLevel = levels[tag]->level;
    SetPreview();
}
void LevelSelect::playCallback(Ref* sender){
    if(selectedLevel != NULL){
        if(Game::myScene == NULL){
            auto scene = Game::createScene();
            Director::getInstance()->pushScene(scene);
            Game::Instance->Reset(selectedLevel);
        }
        else{
            Director::getInstance()->pushScene(Game::myScene);
            Game::Instance->Reset(selectedLevel);
        }
    }
}
void LevelSelect::customCallback(Ref*){
    SetLevelSet(LEVELS_CUSTOM);
    SetLevelSetButtons();
}
void LevelSelect::favoritedCallback(Ref*){
    SetLevelSet(LEVELS_FAVORITED);
    SetLevelSetButtons();
}
void LevelSelect::socialCallback(Ref*){
    SetLevelSet(LEVELS_SOCIAL);
    SetLevelSetButtons();
}
void LevelSelect::risingCallback(Ref*){
    SetLevelSet(LEVELS_RISING);
    SetLevelSetButtons();
}
void LevelSelect::SetLevelSet(int set){
    page = 0;
    currentLevelSet = set;
    LoadLevels();
}
void LevelSelect::SetLevelSetButtons(){
    currentLevelsTab->runAction(MoveTo::create(0.1, Point(currentLevelsTab->getPosition().x, tabHeight)));
    switch(currentLevelSet){
        case LEVELS_RISING:
            currentLevelsTab = risingLevels;
            break;
        case LEVELS_FAVORITED:
            currentLevelsTab = favoritedLevels;
            break;
        case LEVELS_SOCIAL:
            currentLevelsTab = socialLevels;
            break;
        case LEVELS_CUSTOM:
            currentLevelsTab = customLevels;
            break;
    }
    currentLevelsTab->runAction(MoveTo::create(0.1, Point(currentLevelsTab->getPosition().x, tabHeightSelected)));
}
void LevelSelect::Refresh(){
    SetLevelSet(currentLevelSet);
}
void LevelSelect::LoadLevels(){
    for(int x = 0; x < 4; x++){
        levels[x]->SetEnabled(false);
    }
    switch(currentLevelSet){
        case LEVELS_RISING:
            FetchRisingLevels();
            break;
        case LEVELS_FAVORITED:
            FetchFavoritedLevels();
            break;
        case LEVELS_SOCIAL:
            FetchSocialLevels();
            break;
        case LEVELS_CUSTOM:
            FetchCustomLevels();
            break;
    }
};
void LevelSelect::FetchSocialLevels(){
    showLoading();
    Game::SetUpLevels();
    Game::levels[LEVELS_SOCIAL].clear();
    sendMessageWithParams("fetchCustomLevels", Value());
};
void LevelSelect::FetchCustomLevels(){
    showLoading();
    Game::SetUpLevels();
    Game::levels[LEVELS_CUSTOM].clear();
    sendMessageWithParams("fetchCustomLevels", Value());
};
void LevelSelect::FetchRisingLevels(){
    showLoading();
    Game::SetUpLevels();
    Game::levels[LEVELS_RISING].clear();
    sendMessageWithParams("fetchRisingLevels", Value());
};
void LevelSelect::FetchFavoritedLevels(){
    showLoading();
    Game::SetUpLevels();
    Game::levels[LEVELS_FAVORITED].clear();
    sendMessageWithParams("fetchFavoritedLevels", Value());
};
void LevelSelect::showLoading(){
    loading->setVisible(true);
    loading->runAction(Repeat::create(RotateBy::create(1.5, 360),1));
}
void LevelSelect::fetchSocialCallback(Node* sender, Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        ValueMap valueMap = data.asValueMap();
        Game::AddSocialLevel(Level::createWithValueMap(valueMap));
    }
};
void LevelSelect::fetchFavoritedCallback(Node* sender, Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        ValueMap valueMap = data.asValueMap();
        Game::AddFavoritedLevel(Level::createWithValueMap(valueMap));
    }
};
void LevelSelect::fetchRisingCallback(Node* sender, Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        ValueMap valueMap = data.asValueMap();
        Game::AddRisingLevel(Level::createWithValueMap(valueMap));
    }
};
void LevelSelect::fetchCustomCallback(Node* sender, Value data){
    if (!data.isNull() && data.getType() == Value::Type::MAP) {
        printf("Returned with a custom level!");
        ValueMap valueMap = data.asValueMap();
        Game::AddCustomLevel(Level::createWithValueMap(valueMap));
    }
};
void LevelSelect::doneFetching(Node* sender, Value data){
    printf("\nDone Fetching\n");
    loading->setVisible(false);
    loading->stopAllActions();
    if(Game::levels.size() > 0){
        for(int x = page * 4; x < Game::levels[currentLevelSet].size() && x < ((page*4)+4); x++){
            levels[x%4]->SetEnabled(true);
            levels[x%4]->SetLevel(Game::levels[currentLevelSet][x], page);
        }
        selectedLevel = levels[0]->level;
        SetPreview();
    }
};
void LevelSelect::SetPreview(){
    if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Private") == 0){
        uploadButton->setVisible(true);
        uploadButton->setEnabled(true);
        highscoresButton->setVisible(false);
        highscoresButton->setEnabled(false);
    }
    if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Public") == 0){
        uploadButton->setVisible(false);
        uploadButton->setEnabled(false);
        highscoresButton->setVisible(true);
        highscoresButton->setEnabled(true);
    }
    if(currentLevelSet != LEVELS_CUSTOM){
        highscoresButton->setVisible(true);
        highscoresButton->setEnabled(true);
    }
    previewTitle->setVisible(true);
    previewAuthor->setVisible(true);
    previewTitle->setString(selectedLevel->GetName());
    previewAuthor->setString(selectedLevel->GetAuthor());
}
void LevelSelect::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}