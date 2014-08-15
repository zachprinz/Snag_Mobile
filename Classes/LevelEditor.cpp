//
//  LevelEditor.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#include "LevelEditor.h"
#include "MainMenu.h"
#include "Level.h"
#include "Game.h"
#include "LevelSelect.h"
#include "Map.h"

USING_NS_CC;

Scene* LevelEditor::myScene;
LevelEditor* LevelEditor::Instance;

Scene* LevelEditor::createScene(){
    auto scene = Scene::create();
    auto layer = LevelEditor::create();
    myScene = scene;
    scene->addChild(layer);
    return scene;
    Instance = layer;
}

bool LevelEditor::init(){
    if(!Layer::init()){
        return false;
    }
    Instance = this;
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    noticeUp = false;

    preview = new Preview(Rect(0,0,visibleSize.width,visibleSize.height), this, 0.33);
    currentSprite = new Scale9Sprite();
    currentSprite->initWithFile("Sliced.png", Rect(0, 0, 60, 60));
    currentSprite->setContentSize(Size(100, 100));
    currentSprite->setPosition(0,0);
    currentSprite->setGlobalZOrder(0);
    currentSprite->setVisible(false);
    currentSprite->setAnchorPoint(Vec2(0,0));
    
    auto listener = EventListenerTouchAllAtOnce::create();
    //listener->setSwallowTouches(true);
    listener->onTouchesBegan = CC_CALLBACK_2(LevelEditor::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(LevelEditor::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(LevelEditor::onTouchesEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    cocos2d::Vector<MenuItem*> menuItems;
    saveDialog = false;
    
    selectedSprite = MainMenu::CreateButton("LevelEditorSelected.png", Vec2(0,1.0-0.015), Vec2(0,1));
    selectedSprite->setPosition(visibleSize.width / 2.0, selectedSprite->getPosition().y);
    selectedSprite->setAnchorPoint(Point(0.5,1.0));
    auto background = Sprite::create("GRID2.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Point(0,0));
    background->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setGlobalZOrder(-5);
    this->addChild(background);
    menuItems.pushBack(selectedSprite);
    
    float buttonHeightPixels = 155;
    float tempScale = MainMenu::minScreenScale;
    if(tempScale > 1.5)
        tempScale = 1.5;
    float buttonPercent = (buttonHeightPixels * tempScale) / visibleSize.height;
    float leftOverHeight = 1.0 - (5 * buttonPercent);
    float buttonGap = leftOverHeight / 6.0;
    float jump = buttonGap + buttonPercent;
    
    spikeWallSelectButton = MainMenu::CreateButton("LevelEditorSpikeWall.png", this, menu_selector(LevelEditor::SpikeWallSelectCallback), Vec2(0.01,1.0-buttonGap), Vec2(1,0));
    menuItems.pushBack(spikeWallSelectButton);
    wallSelectButton = MainMenu::CreateButton("LevelEditorWall.png", this, menu_selector(LevelEditor::WallSelectCallback), Vec2(0.01,1.0-buttonGap-jump), Vec2(1,0));
    menuItems.pushBack(wallSelectButton);
    spawnerSelectButton = MainMenu::CreateButton("LevelEditorSpawner.png", this, menu_selector(LevelEditor::SpawnerSelectCallback), Vec2(0.01,1.0-buttonGap-(3*jump)), Vec2(1,0));
    menuItems.pushBack(spawnerSelectButton);
    hookSelectButton = MainMenu::CreateButton("LevelEditorHook.png", this, menu_selector(LevelEditor::HookSelectCallback), Vec2(0.01,1.0-buttonGap-(2*jump)), Vec2(1,0));
    menuItems.pushBack(hookSelectButton);
    goalSelectButton = MainMenu::CreateButton("LevelEditorGoal.png", this, menu_selector(LevelEditor::GoalSelectCallback), Vec2(0.01,1.0-buttonGap-(4*jump)), Vec2(1,0));
    menuItems.pushBack(goalSelectButton);
    homeSelectButton = MainMenu::CreateButton("home.png", this, menu_selector(LevelEditor::homeButtonCallback), Vec2(0.9,1.0-buttonGap), Vec2(1,0));
    menuItems.pushBack(homeSelectButton);
    trashSelectButton = MainMenu::CreateButton("trash.png", this, menu_selector(LevelEditor::trashButtonCallback), Vec2(0.9,1.0-buttonGap-(2*jump)), Vec2(1,0));
    menuItems.pushBack(trashSelectButton);
    saveSelectButton = MainMenu::CreateButton("save.png", this, menu_selector(LevelEditor::saveButtonCallback), Vec2(0.9,1.0-buttonGap-(jump)), Vec2(1,0));
    menuItems.pushBack(saveSelectButton);
    eraseSelectButton = MainMenu::CreateButton("LevelEditorErase.png", this, menu_selector(LevelEditor::EraseSelectCallback), Vec2(0.9,1.0-buttonGap-(3*jump)), Vec2(1,0));
    menuItems.pushBack(eraseSelectButton);
    moveSelectButton = MainMenu::CreateButton("LevelEditorMove.png", this, menu_selector(LevelEditor::moveButtonCallback), Vec2(0.9,1.0-buttonGap-(4*jump)), Vec2(1,0));
    menuItems.pushBack(moveSelectButton);
    selectedLabel = MainMenu::CreateLabel("Pan Tool", Vec2(0,1.0-0.015), Vec2(0,1));
    selectedLabel->setGlobalZOrder(0);
    selectedLabel->setPosition(0.5 * visibleSize.width, selectedLabel->getPosition().y);
    selectedLabel->setAnchorPoint(Vec2(0.5,1.0));
        
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(selectedLabel,1);
    
    savePopUp = new PopUp("Save", "Please name your level.", this, menu_selector(LevelEditor::saveAcceptCallback), menu_selector(LevelEditor::saveDeclineCallback),true);
    savePopUp->Add(this);
    
    spawnerPopUp = new PopUp("Error", "Please add at least one\nspawner to your map.", this, menu_selector(LevelEditor::closePopUpCallback));
    spawnerPopUp->Add(this);
    
    hookPopUp = new PopUp("Error", "Please add at least one\nhook to your map.", this, menu_selector(LevelEditor::closePopUpCallback));
    hookPopUp->Add(this);
    
    this->addChild(currentSprite);
    
    currentSelection = NULL;
    currentTool = NO_TOOL;
    
    return true;
}
void LevelEditor::editBoxEditingDidBegin(EditBox *editBox) {
}
void LevelEditor::editBoxEditingDidEnd(EditBox *editBox) {
}
void LevelEditor::editBoxTextChanged(EditBox *editBox, std::string &text) {
}
void LevelEditor::editBoxReturn(EditBox *editBox) {
}
void LevelEditor::closePopUpCallback(Ref*){
    spawnerPopUp->Close();
    hookPopUp->Close();
}
void LevelEditor::SpikeWallSelectCallback(Ref*){
    if(currentSelection != spikeWallSelectButton){
        ResetToolPos();
        currentTool = SPIKE_WALL;
        currentSelection = spikeWallSelectButton;
        SetToolPos();
        selectedLabel->setString("SpikeWall");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
};
void LevelEditor::WallSelectCallback(Ref*){
    if(currentSelection != wallSelectButton){
        ResetToolPos();
        currentTool = WALL;
        currentSelection = wallSelectButton;
        SetToolPos();
        selectedLabel->setString("Wall");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
};
void LevelEditor::SpawnerSelectCallback(Ref*){
    Instance = this;
    if(currentSelection != spawnerSelectButton){
        ResetToolPos();
        currentTool = SPAWNER;
        currentSelection = spawnerSelectButton;
        SetToolPos();
        selectedLabel->setString("Spawner");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
};
void LevelEditor::HookSelectCallback(Ref*){
    if(currentSelection != hookSelectButton){
        ResetToolPos();
        currentSelection = hookSelectButton;
        currentTool = HOOK;
        SetToolPos();
        selectedLabel->setString("Hook");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
};
void LevelEditor::EraseSelectCallback(Ref*){
    if(currentSelection != eraseSelectButton){
        ResetToolPos();
        currentSelection = eraseSelectButton;
        currentTool = ERASE;
        SetToolPos();
        selectedLabel->setString("Erase Tool");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
};
void LevelEditor::homeButtonCallback(Ref* ref){
    if(LevelSelect::myScene == NULL){
        auto scene = LevelSelect::createScene();
        Director::getInstance()->pushScene(scene);
        LevelSelect::Instance->Refresh();
    }
    else{
        Director::getInstance()->pushScene(LevelSelect::myScene);
        LevelSelect::Instance->Refresh();
    }
}
void LevelEditor::saveButtonCallback(Ref* ref){
    bool hasSpawner = false;
    bool hasHook = false;
    for (std::map<int,Entity*>::iterator it=entities.begin(); it!=entities.end(); ++it){
        int type = ((Entity*)it->second)->GetType();
        if(type == SPAWNER)
            hasSpawner = true;
        if(type == HOOK)
            hasHook = true;
    }
    if(!hasSpawner)
        spawnerPopUp->Show();
    if(!hasHook)
        hookPopUp->Show();
    if(hasHook && hasSpawner){
        savePopUp->Show();
        saveDialog = true;
    }
}
void LevelEditor::saveAcceptCallback(Ref* ref){
    name = savePopUp->GetText();
    Export();
    savePopUp->Close();
}
void LevelEditor::saveDeclineCallback(Ref* ref){
    savePopUp->Close();
}
void LevelEditor::moveButtonCallback(Ref* ref){
    if(currentSelection != moveSelectButton){
        ResetToolPos();
        currentSelection = moveSelectButton;
        currentTool = MOVE;
        SetToolPos();
        selectedLabel->setString("Move Object");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
}
void LevelEditor::GoalSelectCallback(Ref* ref){
    if(currentSelection != goalSelectButton){
        ResetToolPos();
        currentSelection = goalSelectButton;
        currentTool = GOAL;
        SetToolPos();
        selectedLabel->setString("Goal Area");
    }
    else{
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
        selectedLabel->setString("Pan Tool");
    }
}
void LevelEditor::selectedButtonCallback(Ref* ref){
    
}
void LevelEditor::playButtonCallback(Ref* ref){
    
}
void LevelEditor::trashButtonCallback(Ref* ref){
    Clear();
}
void LevelEditor::Clear(){
    std::vector<int> ids;
    if(entities.size() > 0){
        for (std::map<int,Entity*>::iterator it=entities.begin(); it!=entities.end(); ++it){
            int entID = it->first;
            ids.push_back(entID);
        }
    }
    for(int x = 0; x < ids.size(); x++){
        int entID = ids[x];
        entities.erase(entID);
        preview->RemoveEntity(entID);
    }
    preview->Reset();
    EnableSpawner();
}
bool LevelEditor::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    if(noticeUp){
        notice->setVisible(false);
        notice->setPosition(Point(notice->getPosition().x, notice->getPosition().y - 1000));
        noticeUp = false;
    }
    else{
        if(!saveDialog){
            touchStart = touch->getLocation();
            if(currentTool == HOOK || currentTool == SPAWNER){
                Entity* tempEnt = (preview->CreateEntity(touchStart, touchStart, currentTool));
                entities[tempEnt->ID] = tempEnt;
                preview->AddEntity(tempEnt);
            } else {
                if(currentTool == NO_TOOL){
                    
                } else {
                    currentSprite->setPosition(touchStart);
                    currentSprite->setContentSize(Size(1,1));
                    currentSprite->setVisible(true);
                }
            }
            return true;
        }
        return false;
    }
    return false;
}
void LevelEditor::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    if(currentTool != ERASE && currentTool != NO_TOOL){
        if(currentTool != SPAWNER && currentTool != HOOK){
            if(std::abs(currentSprite->getContentSize().width) < (0.5 * TILE_SIZE * (1.0/preview->GetScale())) || std::abs(currentSprite->getContentSize().height) < (0.5 * (TILE_SIZE*(1.0/preview->GetScale())))){
                currentSprite->setVisible(false);
            } else{
                Rect bb = currentSprite->getBoundingBox();
                Vec2 minPoint(bb.getMinX(), bb.getMinY());
                Vec2 maxPoint(bb.getMaxX(), bb.getMaxY());
                if(touchStart.x > touchCurrent.x){
                    maxPoint = Vec2(maxPoint.x + 20, maxPoint.y);
                    minPoint = Vec2(minPoint.x - 20, minPoint.y);
                }
                if(touchStart.y > touchCurrent.y){
                    maxPoint = Vec2(maxPoint.x, maxPoint.y + 20);
                    minPoint = Vec2(minPoint.x, minPoint.y - 20);
                }
                Entity* tempEnt = (preview->CreateEntity(minPoint , maxPoint, currentTool));
                entities[tempEnt->ID] = tempEnt;
                preview->AddEntity(tempEnt);
                currentSprite->setVisible(false);
            }
        }
    }
    if(currentTool == ERASE){
        Entity* entToRemove = preview->GetTarget(currentTouches[0]->getLocation());
        preview->RemoveEntity(entToRemove);
        entities.erase(entToRemove->ID);
    }
}
void LevelEditor::onTouchMoved(Touch* touch, Event* event){
    if(currentTool != NO_TOOL && currentTool != ERASE){
        touchCurrent = touch->getLocation();
        if(currentSprite != NULL)
            currentSprite->setContentSize(Size(touchCurrent.x - touchStart.x, touchCurrent.y - touchStart.y));
    }
    if(currentTool == NO_TOOL){
        Vec2 oldTouch = touchStart;
        touchCurrent = touch->getLocation();
        touchStart  = touchCurrent;
        preview->Drag(touchCurrent - oldTouch);
    }
}
void LevelEditor::onTouchesBegan(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event){
    for(int x = 0; x < touches.size(); x++){
        currentTouches.push_back(touches[x]);
    }
    if(currentTouches.size() > 2){
        std::vector<Touch*> tempTouches;
        for(int x = 0; x < currentTouches.size() && x < 2; x++){
            tempTouches.push_back(currentTouches[x]);
        }
        currentTouches.clear();
        currentTouches = tempTouches;
    }
    if(currentTouches.size() == 1){
        onTouchBegan(currentTouches[0], event);
    }
};
void LevelEditor::onTouchesMoved(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event){
    for(int x = 0; x < currentTouches.size(); x++){
        for(int y = 0; y < touches.size(); y++){
            if(currentTouches[x]->getID() == touches[y]->getID()){
                currentTouches[x] = touches[y];
                break;
            }
        }
    }
    if(currentTouches.size() == 1)
        onTouchMoved(currentTouches[0], event);
    if(currentTouches.size() == 2 && currentTool == NO_TOOL)
        preview->onTouchesMoved(currentTouches);
};
void LevelEditor::onTouchesEnded(const std::vector<cocos2d::Touch*> &touches, cocos2d::Event* event){
    if(currentTouches.size() >= 2){
        preview->onTouchesEnded(currentTouches);
        if(touches.size() >= currentTouches.size()){
            currentTouches.clear();
        } else {
            if(touches[0]->getID() == currentTouches[0]->getID())
                currentTouches.erase(currentTouches.begin());
            else
                currentTouches.erase(currentTouches.end()-1);
            std::vector<Touch*> tempTouches = currentTouches;
            currentTouches.clear();
            onTouchesBegan(tempTouches, event);
        }
    } else {
        currentTouches.clear();
        onTouchEnded(touches[0],event);
    }
};
void LevelEditor::SetLevel(Level* lvl){
    Clear();
    currentLevel = lvl;
    std::vector<Entity*> tempEnts = lvl->GetEntities();
    for(int x = 0; x < tempEnts.size(); x++){
        entities[tempEnts[x]->ID] = tempEnts[x];
        preview->AddEntity(tempEnts[x]);
    }
}
void LevelEditor::menuCloseCallback(Ref* pSender){
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    Director::getInstance()->end();
    
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
void LevelEditor::DisableSpawner(){
    if(currentSelection == spawnerSelectButton){
        spawnerSelectButton->setEnabled(false);
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
}
void LevelEditor::EnableSpawner(){
    spawnerSelectButton->setEnabled(true);
}
void LevelEditor::Erase(Vec2 tile){

}
void LevelEditor::SetToolPos(){
    if(currentSelection!= NULL){
    auto setAction = MoveTo::create(0.1, Point(currentSelection->getPosition().x + 20, currentSelection->getPosition().y));
    auto setActionNegative = MoveTo::create(0.1, Point(currentSelection->getPosition().x - 20, currentSelection->getPosition().y));
    switch(currentTool){
        case WALL:
                currentSelection->runAction(setAction);
            break;
        case SPIKE_WALL:
                currentSelection->runAction(setAction);
            break;
        case SPAWNER:
                currentSelection->runAction(setAction);
            break;
        case HOOK:
                currentSelection->runAction(setAction);
            break;
        case NO_TOOL:
            break;
        case ERASE:
                currentSelection->runAction(setActionNegative);
            break;
        case MOVE:
                currentSelection->runAction(setActionNegative);
            break;
        case GOAL:
                currentSelection->runAction(setAction);
            break;
    }
    }
}
void LevelEditor::ResetToolPos(){
    if(currentSelection != NULL){
    auto setAction = MoveTo::create(0.1, Point(currentSelection->getPosition().x + 20, currentSelection->getPosition().y));
    auto setActionNegative = MoveTo::create(0.1, Point(currentSelection->getPosition().x - 20, currentSelection->getPosition().y));
    switch(currentTool){
        case WALL:
                currentSelection->runAction(setActionNegative);
            break;
        case SPIKE_WALL:
                currentSelection->runAction(setActionNegative);
            break;
        case SPAWNER:
                currentSelection->runAction(setActionNegative);
            break;
        case HOOK:
                currentSelection->runAction(setActionNegative);
            break;
        case NO_TOOL:
            break;
        case ERASE:
                currentSelection->runAction(setAction);
            break;
        case MOVE:
                currentSelection->runAction(setAction);
            break;
        case GOAL:
                currentSelection->runAction(setActionNegative);
            break;
    }
    }
}
void LevelEditor::Export(){
    currentLevel->SetName(name);
    currentLevel->Clear();
    for (std::map<int,Entity*>::iterator it=entities.begin(); it!=entities.end(); ++it){
        int entID = ((Entity*)it->second)->ID;
        currentLevel->AddEntity(entities[entID]);
    }
    currentLevel->Save();
}