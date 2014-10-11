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
#include "Highscores.h"
#include "NDKHelper/NDKHelper.h"

USING_NS_CC;

Scene* LevelSelect::myScene;
Vec2 LevelSelect::screenScale;
LevelSelect* LevelSelect::Instance;

Scene* LevelSelect::createScene() {
    auto scene = Scene::create();
    auto layer = LevelSelect::create();
    myScene = scene;
    myScene->retain();
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
    
    auto listener = EventListenerTouchOneByOne::create();
    //listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LevelSelect::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LevelSelect::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LevelSelect::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    

    
    std::map<std::string, SEL_MenuHandler> callbacks;
    callbacks["CustomImage"] = menu_selector(LevelSelect::customCallback);
    callbacks["RisingImage"] = menu_selector(LevelSelect::risingCallback);
    callbacks["SocialImage"] = menu_selector(LevelSelect::socialCallback);
    callbacks["StarImage"] = menu_selector(LevelSelect::favoritedCallback);
    callbacks["Delete"] = menu_selector(LevelSelect::deleteCallback);
    callbacks["Edit"] = menu_selector(LevelSelect::editCallback);
    callbacks["HighScores"] = menu_selector(LevelSelect::highscoresCallback);
    callbacks["Left"] = menu_selector(LevelSelect::scrollLeftCallback);
    callbacks["Right"] = menu_selector(LevelSelect::scrollRightCallback);
    callbacks["Play"] = menu_selector(LevelSelect::playCallback);
    callbacks["Upload"] = menu_selector(LevelSelect::uploadCallback);
    callbacks["NewLevel"] = menu_selector(LevelSelect::newLevelCallback);
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("levelselect", this, callbacks, &menuItems, this);
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    

    scrollview = ui::ScrollView::create();
    scrollview->setContentSize(Size(elements["Inlay"]->getBoundingBox().size.width*1, elements["Inlay"]->getBoundingBox().size.height * 1));
    scrollview->setSize(Size(elements["Inlay"]->getBoundingBox().size.width*1, elements["Inlay"]->getBoundingBox().size.height * 1));
    scrollview->setPosition(Vec2(elements["Inlay"]->getBoundingBox().getMidX(), elements["Inlay"]->getBoundingBox().getMidY()));// + visibleSize.height * 0.0435));
    scrollview->setAnchorPoint(Point(0.5,0.5));
    scrollview->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollview->setInertiaScrollEnabled(true);
    float innerWidth = elements["Inlay"]->getBoundingBox().size.width * 1;
    float innerHeight = elements["Inlay"]->getBoundingBox().size.height * 3;
    scrollview->setInnerContainerSize(Size(innerWidth,innerHeight));
    
    auto pinnedPanel = MainMenu::CreateButton("levelselect", "Pinned_Panel.png");
    pinnedPanel->setPosition(elements["Inlay"]->getBoundingBox().getMidX(), elements["Inlay"]->getBoundingBox().getMaxY());
    pinnedPanel->setAnchorPoint(Vec2(0.5,1));
    
    for(int x = 0; x < 4; x++){
        LevelMenuItem* lvl = new LevelMenuItem(x, scrollview, ((float)innerWidth) / 2.0, elements["Inlay"]->getBoundingBox().size.height*3-pinnedPanel->getBoundingBox().size.height);
        lvl->SetTag(x);
        levels.push_back(lvl);
    }
    
    previewTitle = MainMenu::CreateLabel("Select A", Vec2(0,0), Vec2(0,0));
    previewTitle->setGlobalZOrder(0);
    previewTitle->setPosition(Vec2(elements["LevelTitleBackground"]->getBoundingBox().getMidX(), elements["LevelTitleBackground"]->getBoundingBox().getMidY()));
    previewTitle->setVisible(false);
    this->addChild(previewTitle, 1);
    previewAuthor = MainMenu::CreateLabel("Level", Vec2(0,0), Vec2(0,0));
    previewAuthor->setPosition(Vec2(elements["LevelAuthorBackground"]->getBoundingBox().getMidX(), elements["LevelAuthorBackground"]->getBoundingBox().getMidY()));
    previewAuthor->setGlobalZOrder(0);
    previewAuthor->setVisible(false);
    this->addChild(previewAuthor, 1);
    auto titleLabel = MainMenu::CreateLabel("Level Select", Vec2(0,0), Vec2(0,0));
    titleLabel->setPosition(Vec2(elements["TitlePanel"]->getBoundingBox().getMidX(), elements["TitlePanel"]->getBoundingBox().getMidY()));
    titleLabel->setGlobalZOrder(0);
    titleLabel->setVisible(true);
    this->addChild(titleLabel, 1);
    
    currentLevelsTab = elements["RisingTab"];
    
    this->addChild(scrollview,1);
    elements["Pinned_Panel"]->setVisible(false);//(10);
    

    this->addChild(pinnedPanel, 1);
    
    auto featuredLabelPanel = MainMenu::CreateButton("levelselect", "Featured_Label_Panel.png");

    auto featured_label = MainMenu::CreateLabel("Featured", Vec2(0,0), Vec2(0,0));
    featured_label->setPosition(Vec2(elements["Pinned_Panel"]->getBoundingBox().getMidX(), elements["Pinned_Panel"]->getBoundingBox().getMaxY() - (featuredLabelPanel->getBoundingBox().size.height*0.65/2.0)));
    featured_label->setAnchorPoint(Vec2(0.5,0.5));
    featured_label->setScaleX(featured_label->getScaleX()* 0.6);
    featured_label->setScaleY(featured_label->getScaleY()* 0.5);
    this->addChild(featured_label,1);
    
    for(int x = 0; x < 2; x++){
        LevelMenuItem* lvl = new LevelMenuItem(x, this, ((float)innerWidth) / 2.0 + scrollview->getBoundingBox().getMinX(), elements["Pinned_Panel"]->getBoundingBox().getMaxY() - featuredLabelPanel->getBoundingBox().size.height - (elements["Pinned_Panel"]->getBoundingBox().size.height*0.85 - featuredLabelPanel->getBoundingBox().size.height)/2.0);
        lvl->SetTag(x);
        featuredLevels.push_back(lvl);
    }
    
    loading = Sprite::create("Loading.png");
    loading->setScale(0.5,0.5);
    loading->setPosition(levels[1]->background->getBoundingBox().getMinY(), levels[1]->background->getBoundingBox().getMidX());
    loading->setAnchorPoint(Vec2(0.5, 0.5));
    loading->setVisible(false);
    this->addChild(loading,1);

    
    tabHeight = elements["SocialTab"]->getPosition().y;
    tabHeightSelected = tabHeight - 20;
    currentLevelsTab->setPositionY(tabHeightSelected);
    
    //-- Set Up Newtork Stuff --//
    NDKHelper::addSelector("LevelSelect", "fetchCustomCallback", CC_CALLBACK_2(LevelSelect::fetchCustomCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "fetchSocialCallback", CC_CALLBACK_2(LevelSelect::fetchSocialCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "fetchFavoritedCallback", CC_CALLBACK_2(LevelSelect::fetchFavoritedCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "fetchRisingCallback", CC_CALLBACK_2(LevelSelect::fetchRisingCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "doneFetching", CC_CALLBACK_2(LevelSelect::doneFetching, this), this);
    NDKHelper::addSelector("LevelSelect", "favCallback", CC_CALLBACK_2(LevelSelect::favCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "levelDeletedCallback", CC_CALLBACK_2(LevelSelect::levelDeletedCallback, this), this);
    NDKHelper::addSelector("LevelSelect", "newLevelResponce", CC_CALLBACK_2(LevelSelect::newLevelResponce, this), this);
    //--------------------------//
    
    preview = new Preview(Rect(elements["LevelPreviewBackground"]->getBoundingBox().getMinX() + 10,elements["LevelPreviewBackground"]->getBoundingBox().getMinY() + 10,elements["LevelPreviewBackground"]->getBoundingBox().size.width - 20,elements["LevelPreviewBackground"]->getBoundingBox().size.height-20), this, 0.2);
    hand = MainMenu::CreateButton("Hand.png", Vec2(0,0), Vec2(0,0));
    hand->setPosition(Vec2(elements["LevelPreviewBackground"]->getBoundingBox().getMidX(), elements["LevelPreviewBackground"]->getBoundingBox().getMidY()));
    hand->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(hand,1);
    deletePopUp = new PopUp("Delete Level", "Are you sure you want to\ndelete this level?", this, menu_selector(LevelSelect::deleteAcceptCallback), menu_selector(LevelSelect::deleteDeclineCallback));
    deletePopUp->Add(this);
    goToEdit = false;
    return true;
}
bool LevelSelect::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    touchStart = touch->getLocation();
    touchCurrent = touch->getLocation();
    if(elements["LevelPreviewBackground"]->getBoundingBox().containsPoint(touchStart)){
        isDragging = true;
        hand->setVisible(false);
    }
    return true;
};
void LevelSelect::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    
};
void LevelSelect::onTouchMoved(Touch* touch, Event* event){
    if(isDragging){
        Vec2 oldTouch = touchStart;
        touchCurrent = touch->getLocation();
        touchStart  = touchCurrent;
        preview->Drag(touchCurrent - oldTouch);
    }
};
void LevelSelect::editCallback(Ref*){
    if(selectedLevel != nullptr)
        goToLevelEditor();
}
void LevelSelect::goToLevelEditor(){
    if(LevelEditor::myScene == NULL){
        auto scene = LevelEditor::createScene();
        LevelEditor::Instance->SetLevel(selectedLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else{
        LevelEditor::Instance->SetLevel(selectedLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelEditor::myScene);
        Director::getInstance()->pushScene(transition);
    }
}
void LevelSelect::highscoresCallback(Ref*){
    if(Highscores::myScene == NULL){
        auto scene = Highscores::createScene();
        Highscores::Instance->SetLevel(selectedLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else{
        Highscores::Instance->SetLevel(selectedLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, Highscores::myScene);
        Director::getInstance()->pushScene(transition);
    }
}
void LevelSelect::deleteAcceptCallback(Ref*){
    ValueMap valueMap;
    valueMap["id"] = selectedLevel->GetID();
    valueMap["refresh"] = 1;
    Value parameters = Value(valueMap);
    sendMessageWithParams("deleteLevel", parameters);
    deletePopUp->Close();
}
void LevelSelect::deleteDeclineCallback(Ref*){
    deletePopUp->Close();
}
void LevelSelect::levelDeletedCallback(Node* sender, Value data){
    Refresh();
}
void LevelSelect::uploadCallback(Ref*){
    selectedLevel->makePublic();
    if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Public") == 0){
        elements["Upload"]->setVisible(false);
        elements["Upload"]->setEnabled(false);
        elements["HighScores"]->setVisible(true);
        elements["HighScores"]->setEnabled(true);
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
}
void LevelSelect::newLevelResponce(cocos2d::Node *sender, cocos2d::Value data){
    goToEdit = true;
    if(currentLevelSet != LEVELS_CUSTOM)
        customCallback(nullptr);
    else
        Refresh();
}
void LevelSelect::deleteCallback(Ref*){
    deletePopUp->Show();
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
            auto transition = TransitionFade::create(MainMenu::transitionTime, Game::myScene);
            Director::getInstance()->pushScene(transition);
            Game::Instance->Reset(selectedLevel);
        }
        else{
            auto transition = TransitionFade::create(MainMenu::transitionTime, Game::myScene);
            Director::getInstance()->pushScene(transition);
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
            currentLevelsTab = elements["RisingTab"];
            break;
        case LEVELS_FAVORITED:
            currentLevelsTab = elements["StarredTab"];
            break;
        case LEVELS_SOCIAL:
            currentLevelsTab = elements["SocialTab"];
            break;
        case LEVELS_CUSTOM:
            currentLevelsTab = elements["CustomTab"];
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
    loading->runAction(Repeat::create(RotateBy::create(1.5, 360),5));
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
	log("\nDone Fetching");
    loading->setVisible(false);
    loading->stopAllActions();
    if(Game::levels.size() > 0){
        for(int x = page * 4; x < Game::levels[currentLevelSet].size() && x < ((page*4)+4); x++){
        	log("\tIterating through levels");
            levels[x%4]->SetEnabled(true);
            levels[x%4]->SetLevel(Game::levels[currentLevelSet][x], page);
        }
        selectedLevel = levels[0]->level;
        SetPreview();
    }
    if(goToEdit){
        goToEdit = false;
        for(int x = 0; x < Game::levels[currentLevelSet].size(); x++){
            if(Game::levels[currentLevelSet][x]->GetName().compare("qq36q81q") == 0){
                selectedLevel = Game::levels[currentLevelSet][x];
                SetPreview();
                break;
            }
        }
        editCallback(nullptr);
    }
};
void LevelSelect::SetPreview(){
	log("Setting Preview");
    if(selectedLevel != nullptr){
    	log("\tFound selected level");
        preview->Clear();
        hand->setVisible(true);
        log("\tCleared level set");
        if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Private") == 0){
            elements["Upload"]->setVisible(true);
            elements["Upload"]->setEnabled(true);
            elements["HighScores"]->setVisible(false);
            elements["HighScores"]->setEnabled(false);
        }
        if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Public") == 0){
            elements["Upload"]->setVisible(false);
            elements["Upload"]->setEnabled(false);
            elements["HighScores"]->setVisible(true);
            elements["HighScores"]->setEnabled(true);
        }
        if(currentLevelSet != LEVELS_CUSTOM){
            elements["HighScores"]->setVisible(true);
            elements["HighScores"]->setEnabled(true);
        }
        previewTitle->setVisible(true);
        previewAuthor->setVisible(true);
        previewTitle->setString(selectedLevel->GetName());
        previewTitle->setScale(1.0);
        while(previewTitle->getBoundingBox().size.width >= 320){
            previewTitle->setScaleX(previewTitle->getScaleX() - 0.05);
            previewTitle->setScaleY(previewTitle->getScaleY() - 0.05);
        }
        previewAuthor->setString(selectedLevel->GetAuthor());
        log("\tFinished changing Buttons");
        preview->Reset();
        std::vector<Entity*> tempEnts = selectedLevel->GetEntities();
        log("\tEntering Loop");
        for(int x = 0; x < tempEnts.size(); x++){
        	log("\t\tAdding Entity");
            preview->AddEntity(tempEnts[x]);
        }
        log("\t\tOut of Loop");
    }
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
