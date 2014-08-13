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

    preview = new Preview(Rect(0,0,visibleSize.width,visibleSize.height), this, 1.0);
    currentSprite = new Scale9Sprite();
    currentSprite->initWithFile("Sliced.png", Rect(0, 0, 60, 60));
    currentSprite->setContentSize(Size(100, 100));
    currentSprite->setPosition(0,0);
    currentSprite->setGlobalZOrder(0);
    currentSprite->setVisible(false);
    currentSprite->setAnchorPoint(Vec2(0,0));
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LevelEditor::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LevelEditor::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(LevelEditor::onTouchMoved, this);
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
    
    notice = Label::createWithBMFont("dimbo.fnt", "Please add at least one spawner to your level!", TextHAlignment::CENTER);
    notice->setScale(0.45);
    notice->setVisible(false);
    this->addChild(notice);
        
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(selectedLabel,1);
    
    savePopUp = new PopUp("Save", "Please name your level.", this, menu_selector(LevelEditor::saveAcceptCallback), menu_selector(LevelEditor::saveDeclineCallback),true);
    savePopUp->Add(this);
    this->addChild(currentSprite);
    
    currentSelection = NULL;
    
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
    //for(int x = 0; x < mapObjects.size(); x++){
        //if(mapObjects[x]->GetType() == SPAWNER){
        //   hasSpawner = true;
         //   break;
        //}
    //}
    if(!hasSpawner){
        if(!noticeUp){
            notice->setVisible(true);
            notice->setPosition(Point(notice->getPosition().x, notice->getPosition().y + 1000));
            noticeUp = true;
        }
    }
    else{
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
    /*for(int x = 0; x < mapObjects.size(); x++){
        this->removeChild(mapObjects[x]->GetSprite());
    }
    mapObjects.clear();
    originTile = Vec2(0,0);
    MapObject::origin = originTile;
    EnableSpawner();*/
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
            currentSprite->setPosition(touchStart);
            currentSprite->setContentSize(Size(1,1));
            currentSprite->setVisible(true);
            return true;
        }
        return false;
    }
    return false;
}
void LevelEditor::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    if(currentTool != ERASE && currentTool != NO_TOOL){
        if(currentTool != SPAWNER && currentTool != HOOK){
            if(currentSprite->getContentSize().width < (TILE_SIZE) || currentSprite->getContentSize().height < (TILE_SIZE)){
                currentSprite->setVisible(false);
            } else{
                Entity* tempEnt = (preview->CreateEntity(touchStart, touchCurrent, currentTool));
                entities[tempEnt->ID] = tempEnt;
                preview->AddEntity(tempEnt);
                currentSprite->setVisible(false);
            }

        } else {
            Entity* tempEnt = (preview->CreateEntity(touchStart, touchCurrent, currentTool));
            entities[tempEnt->ID] = tempEnt;
            preview->AddEntity(tempEnt);
            currentSprite->setVisible(false);
        }
    }
}
void LevelEditor::onTouchMoved(Touch* touch, Event* event){
    if(currentTool != NO_TOOL && currentTool != ERASE){
        touchCurrent = touch->getLocation();
        if(currentSprite != NULL)
            currentSprite->setContentSize(Size(touchCurrent.x - touchStart.x, touchCurrent.y - touchStart.y));
    }
    /*if(currentTool == NO_TOOL){
        touchCurrent = PixelToTile(touch->getLocation());
        originTile = Vec2(originTile.x + (touchCurrent.x - touchStart.x), originTile.y + (touchCurrent.y - touchStart.y));
        touchCurrent = PixelToTile(touch->getLocation());
        for(int x = 0; x < mapObjects.size(); x++){
            mapObjects[x]->SetOriginTile(originTile);
        }
        printf("OriginTile: (%f, %f)\n", originTile.x, originTile.y);
        printf("StartTile: (%f,%f)\n",touchStart.x, touchStart.y);
        printf("EndTile: (%f,%f)\n",touchCurrent.x, touchCurrent.y);
        touchStart = touchCurrent;
    }*/
}
void LevelEditor::SetLevel(Level* lvl){
    /*currentLevel = lvl;
    Clear();
    mapObjects = lvl->GetMapObjects();
    for(int x = 0; x < mapObjects.size(); x++){
        this->addChild(mapObjects[x]->GetSprite());
    }*/
}
//void LevelEditor::AddMapObject(MapObject* mapObject){

//}
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
ResetToolPos();        currentTool = NO_TOOL;
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
    /*currentLevel->SetName(name);
    for(int x = 0; x < mapObjects.size(); x++){
        currentLevel->AddEntity(mapObjects[x]);
    }
    currentLevel->Save();*/
}