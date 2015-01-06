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
    NUMBER_OF_LEVELS = 20;
    currentLevelSet = LEVELS_RISING;
    page = 0;
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    std::map<std::string, SEL_MenuHandler> callbacks;
    callbacks["CustomImage"] = menu_selector(LevelSelect::customCallback);
    callbacks["RisingImage"] = menu_selector(LevelSelect::risingCallback);
    callbacks["SocialImage"] = menu_selector(LevelSelect::socialCallback);
    callbacks["StarredImage"] = menu_selector(LevelSelect::favoritedCallback);
    callbacks["Delete"] = menu_selector(LevelSelect::deleteCallback);
    callbacks["Edit"] = menu_selector(LevelSelect::editCallback);
    callbacks["Highscores"] = menu_selector(LevelSelect::highscoresCallback);
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
    scrollview->setPosition(Vec2(elements["Inlay"]->getBoundingBox().getMidX(), elements["Inlay"]->getBoundingBox().getMidY()));
    scrollview->setAnchorPoint(Point(0.5,0.5));
    scrollview->setDirection(ui::ScrollView::Direction::VERTICAL);
    scrollview->setInertiaScrollEnabled(true);
    float innerWidth = elements["Inlay"]->getBoundingBox().size.width * 1;
    float innerHeight = elements["Inlay"]->getBoundingBox().size.height * 3;
    scrollview->setInnerContainerSize(Size(innerWidth,innerHeight));
    scrollPercent = 0;
    
    auto pinnedPanel = MainMenu::CreateButton("levelselect", "Pinned_Panel.png");
    pinnedPanel->setPosition(elements["Inlay"]->getBoundingBox().getMidX(), elements["Inlay"]->getBoundingBox().getMaxY());
    pinnedPanel->setAnchorPoint(Vec2(0.5,1));
    this->removeChild(elements["NewLevel"]);
    elements["NewLevel"]->removeFromParent();
    
    for(int x = 0; x < NUMBER_OF_LEVELS; x++){
        LevelMenuItem* lvl = new LevelMenuItem(x, scrollview, ((float)innerWidth) / 2.0, elements["Inlay"]->getBoundingBox().size.height*3-pinnedPanel->getBoundingBox().size.height);
        lvl->SetTag(x);
        levels.push_back(lvl);
    }
    
    previewTitle = MainMenu::CreateLabel("Select A", 1);
    previewTitle->setGlobalZOrder(0);
    previewTitle->setPosition(Vec2(elements["LevelTitleBackground"]->getBoundingBox().getMidX(), elements["LevelTitleBackground"]->getBoundingBox().getMidY()));
    previewTitle->setVisible(false);
    this->addChild(previewTitle, 1);
    previewAuthor = MainMenu::CreateLabel("Level", 1);
    previewAuthor->setPosition(Vec2(elements["LevelAuthorBackground"]->getBoundingBox().getMidX(), elements["LevelAuthorBackground"]->getBoundingBox().getMidY()));
    previewAuthor->setGlobalZOrder(0);
    previewAuthor->setVisible(false);
    this->addChild(previewAuthor, 1);
    auto titleLabel = MainMenu::CreateLabel("Level Select", 2);
    titleLabel->setPosition(Vec2(elements["TitlePanel"]->getBoundingBox().getMidX(), elements["TitlePanel"]->getBoundingBox().getMidY()));
    titleLabel->setGlobalZOrder(0);
    titleLabel->setVisible(true);
    this->addChild(titleLabel, 1);
    
    currentLevelsTab = elements["RisingTab"];
    currentLevelsImage = elements["RisingImage"];
    
    this->addChild(scrollview,1);
    elements["Pinned_Panel"]->setVisible(false);//(10);
    

    this->addChild(pinnedPanel, 1);
    this->addChild(elements["NewLevel"],1);
    elements["NewLevel"]->setVisible(false);
    newLevelLabel = MainMenu::CreateLabel("New Level", 2);
    newLevelLabel->setPosition(Vec2(elements["NewLevel"]->getBoundingBox().getMidX(), elements["NewLevel"]->getBoundingBox().getMidY()));
    newLevelLabel->setAnchorPoint(Vec2(0.5,0.5));
    newLevelLabel->setVisible(false);
    this->addChild(newLevelLabel,1);
    
    auto featuredLabelPanel = MainMenu::CreateButton("levelselect", "Featured_Label_Panel.png");

    auto featured_label = MainMenu::CreateLabel("Featured", 0);
    featured_label->setPosition(Vec2(elements["Pinned_Panel"]->getBoundingBox().getMidX(), elements["Pinned_Panel"]->getBoundingBox().getMaxY() - (featuredLabelPanel->getBoundingBox().size.height*0.65/2.0)));
    featured_label->setAnchorPoint(Vec2(0.5,0.5));
    this->addChild(featured_label,1);
    
    for(int x = 0; x < 2; x++){
        LevelMenuItem* lvl = new LevelMenuItem(x, this, ((float)innerWidth) / 2.0 + scrollview->getBoundingBox().getMinX(), elements["Pinned_Panel"]->getBoundingBox().getMaxY() - featuredLabelPanel->getBoundingBox().size.height - (elements["Pinned_Panel"]->getBoundingBox().size.height*0.85 - featuredLabelPanel->getBoundingBox().size.height)/2.0);
        lvl->SetTag(x);
        featuredLevels.push_back(lvl);
    }
    
    loading = elements["Loading"];
    this->removeChild(loading);
    loading->removeFromParent();
    this->addChild(loading,1);
    Vec2 loadingMidPosition(loading->getBoundingBox().getMidX(), loading->getBoundingBox().getMidY());
    loading->setAnchorPoint(Vec2(0.5,0.5));
    loading->setPosition(loadingMidPosition);
    loading->setVisible(false);
    
    tabHeight = elements["SocialTab"]->getPosition().y;
    tabHeightSelected = tabHeight - (20 * (visibleSize.height / 1080.0));
    imageHeight = elements["SocialImage"]->getPosition().y;
    imageHeightSelected = imageHeight - (20 * (visibleSize.height / 1080.0));
    currentLevelsTab->setPositionY(tabHeightSelected);
    currentLevelsImage->setPositionY(imageHeightSelected);
    
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
    
    preview = new Preview(Rect(elements["LevelPreviewBackground"]->getBoundingBox().getMinX() + 10,elements["LevelPreviewBackground"]->getBoundingBox().getMinY() + 10,elements["LevelPreviewBackground"]->getBoundingBox().size.width - 20,elements["LevelPreviewBackground"]->getBoundingBox().size.height-20), this, 0.3 * ((elements["LevelPreviewBackground"]->getBoundingBox().size.width / 612.0)));
    hand = elements["Hand"];
    this->removeChild(elements["Hand"]);
    hand->removeFromParent();
    this->addChild(hand,1);
    
    deletePopUp = new PopUp("Delete Level", "Are you sure you want to\ndelete this level?", this, menu_selector(LevelSelect::deleteAcceptCallback), menu_selector(LevelSelect::deleteDeclineCallback));
    deletePopUp->Add(this);
    goToEdit = false;
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LevelSelect::onTouchBegan, this);
    listener->onTouchMoved = CC_CALLBACK_2(LevelSelect::onTouchMoved, this);
    listener->onTouchEnded = CC_CALLBACK_2(LevelSelect::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,scrollview);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), preview->clipNode);
    
    return true;
}
bool LevelSelect::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    touchStart = touch->getLocation();
    touchCurrent = touch->getLocation();
    if(elements["LevelPreviewBackground"]->getBoundingBox().containsPoint(touchStart)){
        isDragging = true;
        hand->setVisible(false);
        return true;
    }
    if(elements["NewLevel"]->getBoundingBox().containsPoint(touchStart)){
        if(elements["NewLevel"]->isVisible()){
            newLevelCallback(NULL);
            return false;
        }
        for(int x = 0; x < featuredLevels.size(); x++){
            if(featuredLevels[x]->background->getBoundingBox().containsPoint(touchStart)){
                selectedLevel = featuredLevels[x]->level;
                SetPreview();
                return false;
            }
        }
        return false;
    }
    if(scrollview->getBoundingBox().containsPoint(touchStart) && !deletePopUp->visible){
        recordScrollDistance = true;
        scrollPressDistance = 0;
        return true;
    }
    return false;
};
void LevelSelect::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    if(recordScrollDistance && scrollPressDistance < 20){
        Vec2 touchLocation = touch->getLocation();
        touchLocation.y-=scrollview->getInnerContainer()->getPosition().y;
        for(int x = 0; x < levels.size(); x++){
            Rect levelRect = levels[x]->background->getBoundingBox();
            if(levels[x]->background->getBoundingBox().containsPoint(touchLocation)){
                selectedLevel = levels[x]->level;
                SetPreview();
            }
        }
    }
    isDragging = false;
    recordScrollDistance = false;
};
void LevelSelect::onTouchMoved(Touch* touch, Event* event){
    if(isDragging){
        Vec2 oldTouch = touchStart;
        touchCurrent = touch->getLocation();
        touchStart  = touchCurrent;
        preview->Drag(touchCurrent - oldTouch);
    }
    if(recordScrollDistance){
        Vec2 oldTouch = touchStart;
        touchCurrent = touch->getLocation();
        touchCurrent.x = oldTouch.x;
        float distance = touchCurrent.getDistance(oldTouch);
        scrollPressDistance += distance;
        if(touchCurrent.y - oldTouch.y != 0){
            float scrollDifference = ((touchCurrent.y - oldTouch.y)/(fabs(touchCurrent.y - oldTouch.y))) * (distance/scrollview->getBoundingBox().size.height);
            if(scrollDifference != NAN)
                scrollPercent += scrollDifference;
            if(scrollPercent > 1.0)
                scrollPercent = 1.0;
            if(scrollPercent < 0.00)
                scrollPercent = 0.00;
            scrollview->scrollToPercentVertical(scrollPercent*75.0,0.1, true);
        }
        touchStart  = touchCurrent;
    }
};
void LevelSelect::editCallback(Ref*){
    if(selectedLevel != nullptr && selectedLevel->GetAuthor().compare(MainMenu::Instance->username) == 0)
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
        //elements["HighScores"]->setVisible(true);
        //elements["HighScores"]->setEnabled(true);
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
    Level* newLevel = new Level(true);
    selectedLevel = newLevel;
    goToLevelEditor();
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
    if(currentLevelSet == LEVELS_CUSTOM){
        SetLevelSet(LEVELS_CUSTOM);
    } else {
        SetLevelSet(LEVELS_CUSTOM);
        SetLevelSetButtons();
    }
}
void LevelSelect::favoritedCallback(Ref*){
    if(currentLevelSet == LEVELS_FAVORITED){
        SetLevelSet(LEVELS_FAVORITED);
    } else {
        SetLevelSet(LEVELS_FAVORITED);
        SetLevelSetButtons();
    }
}
void LevelSelect::socialCallback(Ref*){
    if(currentLevelSet == LEVELS_SOCIAL){
        SetLevelSet(LEVELS_SOCIAL);
    } else {
        SetLevelSet(LEVELS_SOCIAL);
        SetLevelSetButtons();
    }
}
void LevelSelect::risingCallback(Ref*){
    if(currentLevelSet == LEVELS_RISING){
        SetLevelSet(LEVELS_RISING);
    } else {
        SetLevelSet(LEVELS_RISING);
        SetLevelSetButtons();
    }
}
void LevelSelect::SetLevelSet(int set){
    page = 0;
    currentLevelSet = set;
    LoadLevels();
}
void LevelSelect::SetLevelSetButtons(){
    currentLevelsTab->runAction(MoveTo::create(0.1, Point(currentLevelsTab->getPosition().x, tabHeight)));
    currentLevelsImage->runAction(MoveTo::create(0.1, Point(currentLevelsImage->getPosition().x, imageHeight)));
    switch(currentLevelSet){
        case LEVELS_RISING:
            currentLevelsTab = elements["RisingTab"];
            currentLevelsImage = elements["RisingImage"];
            break;
        case LEVELS_FAVORITED:
            currentLevelsTab = elements["StarredTab"];
            currentLevelsImage = elements["StarredImage"];
            break;
        case LEVELS_SOCIAL:
            currentLevelsTab = elements["SocialTab"];
            currentLevelsImage = elements["SocialImage"];
            break;
        case LEVELS_CUSTOM:
            currentLevelsTab = elements["CustomTab"];
            currentLevelsImage = elements["CustomImage"];
            break;
    }
    currentLevelsImage->runAction(MoveTo::create(0.1, Point(currentLevelsImage->getPosition().x, imageHeightSelected)));
    currentLevelsTab->runAction(MoveTo::create(0.1, Point(currentLevelsTab->getPosition().x, tabHeightSelected)));
}
void LevelSelect::Refresh(){
    SetLevelSet(currentLevelSet);
}
void LevelSelect::LoadLevels(){
    for(int x = 0; x < NUMBER_OF_LEVELS; x++){
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
    log("\nFetching");
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
        ValueMap valueMap = data.asValueMap();
        Game::AddCustomLevel(Level::createWithValueMap(valueMap));
    }
};
void LevelSelect::doneFetching(Node* sender, Value data){
	log("\nDone Fetching");
    loading->setVisible(false);
    loading->stopAllActions();
    if(Game::levels.size() > 0){
        for(int x = page * NUMBER_OF_LEVELS; x < Game::levels[currentLevelSet].size() && x < ((page*NUMBER_OF_LEVELS)+NUMBER_OF_LEVELS); x++){
        	log("\tIterating through levels");
            levels[x%NUMBER_OF_LEVELS]->SetEnabled(true);
            levels[x%NUMBER_OF_LEVELS]->SetLevel(Game::levels[currentLevelSet][x], page);
        }
        selectedLevel = levels[0]->level;
        SetPreview();
    }
    SetSelectCustomLevels();
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
        printf("\nCurrent User: %s\nLevel Owner: %s", MainMenu::Instance->username.c_str(), selectedLevel->GetAuthor().c_str());
        if(selectedLevel->GetAuthor().compare(MainMenu::Instance->username) != 0){
            elements["Edit"]->setVisible(false);
            elements["Delete"]->setVisible(false);
        }
        if(selectedLevel->GetAuthor().compare(MainMenu::Instance->username) == 0){
            elements["Edit"]->setVisible(true);
            elements["Delete"]->setVisible(true);
        }
        if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Private") == 0){
            elements["Upload"]->setVisible(true);
            elements["Upload"]->setEnabled(true);
            elements["Highscores"]->setVisible(false);
            elements["Highscores"]->setEnabled(false);
        }
        if(currentLevelSet == LEVELS_CUSTOM && selectedLevel->GetStatus().compare("Public") == 0){
            elements["Upload"]->setVisible(false);
            elements["Upload"]->setEnabled(false);
            elements["Highscores"]->setVisible(true);
            elements["Highscores"]->setEnabled(true);
        }
        if(currentLevelSet != LEVELS_CUSTOM){
            elements["Highscores"]->setVisible(true);
            elements["Highscores"]->setEnabled(true);
            elements["Upload"]->setVisible(false);
            elements["Upload"]->setEnabled(false);
        }
        previewTitle->setVisible(true);
        previewAuthor->setVisible(true);
        previewTitle->setString(selectedLevel->GetName());
        previewAuthor->setString(selectedLevel->GetAuthor());
        log("\tFinished changing Buttons");
        preview->Reset();
        std::vector<Entity*> tempEnts = selectedLevel->GetEntities();
        log("\tEntering Loop");
        for(int x = 0; x < tempEnts.size(); x++){
        	log("\t\tAdding Entity");
            preview->AddEntity(tempEnts[x]);
        }
        preview->SetOrigin(Vec2(0,0));
        log("\t\tOut of Loop");
    }
}
void LevelSelect::SetSelectCustomLevels(){
    if(currentLevelSet != LEVELS_CUSTOM){
        for(int x = 0; x < featuredLevels.size(); x++){
            featuredLevels[x]->SetEnabled(true);
        }
        elements["NewLevel"]->setVisible(false);
        newLevelLabel->setVisible(false);
    } else {
        for(int x = 0; x < featuredLevels.size(); x++){
            featuredLevels[x]->SetEnabled(false);
        }
        elements["NewLevel"]->setVisible(true);
        newLevelLabel->setVisible(true);
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
