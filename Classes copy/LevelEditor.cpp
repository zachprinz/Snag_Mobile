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
#include "Board.h"
#include "HelloWorldScene.h"

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
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    originTile = Vec2(0,0);
    noticeUp = false;

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
    selectedLabel->setGlobalZOrder(2);
    selectedLabel->setPosition(0.5 * visibleSize.width, selectedLabel->getPosition().y);
    selectedLabel->setAnchorPoint(Vec2(0.5,1.0));
    
    notice = Label::createWithBMFont("dimbo.fnt", "Please add at least one spawner to your level!", TextHAlignment::CENTER);
    notice->setScale(0.45);
    notice->setVisible(false);
    this->addChild(notice);
    
    cocos2d::Vector<MenuItem*> saveMenuItems;
    shade = MainMenu::CreateButton("shade.png", Vec2(0,0), Vec2(0,0));
    shade->setScale(1,1);
    shade->setScale(visibleSize.width / shade->getBoundingBox().size.width, visibleSize.height / shade->getBoundingBox().size.height);
    saveMenuItems.pushBack(shade);
    saveAcceptButton = MainMenu::CreateButton("check.png", this, menu_selector(LevelEditor::saveAcceptCallback), Vec2(0.25,1.0-0.3), Vec2(1,0));
    saveMenuItems.pushBack(saveAcceptButton);
    saveDeclineButton = MainMenu::CreateButton("x.png", this, menu_selector(LevelEditor::saveDeclineCallback), Vec2(0.75,1.0-0.3), Vec2(1,0));
    saveMenuItems.pushBack(saveDeclineButton);
    saveButtonsOnY = saveAcceptButton->getPosition().y;
    saveButtonsOffY = -500;
    saveAcceptButton->setPosition(saveAcceptButton->getPosition().x, saveButtonsOffY);
    saveDeclineButton->setPosition(saveDeclineButton->getPosition().x, saveButtonsOffY);
    
    Size editBoxSize = Size(750*MainMenu::screenScale.x, 100 * MainMenu::screenScale.y);
    Scale9Sprite* nameBoxBG = Scale9Sprite::create("line.png");
    nameBoxBG->setContentSize(editBoxSize);
    nameBox = EditBox::create(Size(350,50), nameBoxBG);
    nameBox->setPosition(Point(visibleSize.width / 2.0, visibleSize.height / 2.0 + 100 + 1000));
    nameBox->setFontSize(30);
    nameBox->setMaxLength(18);
    nameBox->setPlaceHolder("level name");
    nameBox->setFontColor(Color3B::WHITE);
    this->addChild(nameBox);
    
    menu = Menu::createWithArray(menuItems);
    saveMenu = Menu::createWithArray(saveMenuItems);
    saveMenu->setAnchorPoint(Point(0.0,0.0));
    saveMenu->setPosition(0,0);
    saveMenu->setGlobalZOrder(5);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(saveMenu,1);
    this->addChild(selectedLabel);
    
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
    Director::getInstance()->pushScene(MainMenu::myScene);
}
void LevelEditor::saveButtonCallback(Ref* ref){
    bool hasSpawner = false;
    for(int x = 0; x < mapObjects.size(); x++){
        if(mapObjects[x]->GetType() == SPAWNER){
            hasSpawner = true;
            break;
        }
    }
    if(!hasSpawner){
        if(!noticeUp){
            notice->setVisible(true);
            notice->setPosition(Point(notice->getPosition().x, notice->getPosition().y + 1000));
            noticeUp = true;
        }
    }
    else{
    shade->setAnchorPoint(Point(0,0));
    saveDeclineButton->setPosition(saveDeclineButton->getPosition().x, saveButtonsOnY);
    saveAcceptButton->setPosition(saveAcceptButton->getPosition().x, saveButtonsOnY);
        
    
    moveSelectButton->setEnabled(false);
    eraseSelectButton->setEnabled(false);
    hookSelectButton->setEnabled(false);
    wallSelectButton->setEnabled(false);
    spikeWallSelectButton->setEnabled(false);
    spawnerSelectButton->setEnabled(false);
    goalSelectButton->setEnabled(false);
    homeSelectButton->setEnabled(false);
    trashSelectButton->setEnabled(false);
    saveSelectButton->setEnabled(false);
    saveAcceptButton->setEnabled(true);
    saveDeclineButton->setEnabled(true);
    
    moveSelectButton->setVisible(false);
    eraseSelectButton->setVisible(false);
    hookSelectButton->setVisible(false);
    wallSelectButton->setVisible(false);
    spikeWallSelectButton->setVisible(false);
    spawnerSelectButton->setVisible(false);
    goalSelectButton->setVisible(false);
    homeSelectButton->setVisible(false);
    trashSelectButton->setVisible(false);
    saveSelectButton->setVisible(false);
    selectedSprite->setVisible(false);
    selectedLabel->setVisible(false);
    
    saveDialog = true;
    
    nameBox->setPosition(Point(nameBox->getPosition().x, nameBox->getPosition().y - 1000));
    }
}
void LevelEditor::saveAcceptCallback(Ref* ref){
    name = nameBox->getText();
    Export();
    ResetSaveDialog();
}
void LevelEditor::saveDeclineCallback(Ref* ref){
    ResetSaveDialog();
}
void LevelEditor::ResetSaveDialog(){
    shade->setAnchorPoint(Point(0,1.0));
    saveDeclineButton->setPosition(saveDeclineButton->getPosition().x, saveButtonsOffY);
    saveAcceptButton->setPosition(saveAcceptButton->getPosition().x, saveButtonsOffY);
    
    moveSelectButton->setEnabled(true);
    eraseSelectButton->setEnabled(true);
    hookSelectButton->setEnabled(true);
    wallSelectButton->setEnabled(true);
    spikeWallSelectButton->setEnabled(true);
    spawnerSelectButton->setEnabled(true);
    goalSelectButton->setEnabled(true);
    homeSelectButton->setEnabled(true);
    trashSelectButton->setEnabled(true);
    saveSelectButton->setEnabled(true);
    saveAcceptButton->setEnabled(false);
    saveDeclineButton->setEnabled(false);
    
    moveSelectButton->setVisible(true);
    eraseSelectButton->setVisible(true);
    hookSelectButton->setVisible(true);
    wallSelectButton->setVisible(true);
    spikeWallSelectButton->setVisible(true);
    spawnerSelectButton->setVisible(true);
    goalSelectButton->setVisible(true);
    homeSelectButton->setVisible(true);
    trashSelectButton->setVisible(true);
    saveSelectButton->setVisible(true);
    selectedSprite->setVisible(true);
    selectedLabel->setVisible(true);
    
    saveDialog = false;
    
    nameBox->setPosition(Point(nameBox->getPosition().x, nameBox->getPosition().y + 1000));
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
    for(int x = 0; x < mapObjects.size(); x++){
        this->removeChild(mapObjects[x]->GetSprite());
    }
    mapObjects.clear();
    originTile = Vec2(0,0);
    MapObject::origin = originTile;
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
    touchStart = PixelToTile(touch->getLocation());
    switch(currentTool){
        case WALL:{
            MapObject* mapObject = new MapObject(touchStart, currentTool);
            currentMapObject = mapObject;
            AddMapObject(mapObject);
            break;
        }
        case SPIKE_WALL:{
            MapObject* mapObject = new MapObject(touchStart, currentTool);
            currentMapObject = mapObject;
            AddMapObject(mapObject);
            break;
        }
        case GOAL:{
            MapObject* mapObject = new MapObject(touchStart, currentTool);
            currentMapObject = mapObject;
            AddMapObject(mapObject);
            break;
        }
        case SPAWNER:{
            MapObject* mapObject = new MapObject(touchStart, currentTool);
            currentMapObject = mapObject;
            AddMapObject(mapObject);
            break;
        }
        case HOOK:{
            MapObject* mapObject = new MapObject(touchStart, currentTool);
            currentMapObject = mapObject;
            AddMapObject(mapObject);
            break;
        }
        case NO_TOOL:
            break;
        case ERASE:
            Erase(touchStart);
            break;
    }
    return true;
    }
    return false;
    }
    return false;
}
void LevelEditor::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    if(currentTool != ERASE && currentTool != NO_TOOL && currentTool != SPAWNER && currentTool != HOOK){
        if(currentMapObject->GetSprite()->getBoundingBox().size.width < TILE_SIZE || currentMapObject->GetSprite()->getBoundingBox().size.height < TILE_SIZE){
            for(int x = 0; x < mapObjects.size(); x++){
                if(currentMapObject == mapObjects[x]){
                    Board::Print("Deleted object");
                    this->removeChild(mapObjects[x]->GetSprite());
                    mapObjects.erase(mapObjects.begin() + x);
                    break;
                }
            }
        }
    }
}
void LevelEditor::onTouchMoved(Touch* touch, Event* event){
    if(currentTool != NO_TOOL && currentTool != ERASE){
        touchCurrent = PixelToTile(touch->getLocation());
        if(currentMapObject != NULL)
            currentMapObject->UpdateEndCoord(touchCurrent);
        printf("Tile: (%f,%f)\n",touchCurrent.x, touchCurrent.y);
    }
    if(currentTool == NO_TOOL){
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

    }
}
Vec2 LevelEditor::PixelToTile(Point pos){
    Vec2 ret = Vec2(floor((float)pos.x / TILE_SIZE), floor((float)pos.y / TILE_SIZE));
    ret.x += originTile.x;
    ret.y += originTile.y;
    return ret;
}

void LevelEditor::AddMapObject(MapObject* mapObject){
    mapObjects.push_back(mapObject);
    this->addChild(mapObject->GetSprite());
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
ResetToolPos();        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
}
void LevelEditor::EnableSpawner(){
    spawnerSelectButton->setEnabled(true);
}

void LevelEditor::Erase(Vec2 tile){
    Vec2 pixelPos = Vec2((tile.x - originTile.x + 0.5) * TILE_SIZE, (tile.y - originTile.y + 0.5) * TILE_SIZE);
    for(int x = 0; x < mapObjects.size(); x++){
        if(mapObjects[x]->GetSprite()->getBoundingBox().containsPoint(pixelPos)){
            if(mapObjects[x]->GetType() == SPAWNER)
                EnableSpawner();
            this->removeChild(mapObjects[x]->GetSprite());
            mapObjects.erase(mapObjects.begin() + x);
            break;
        }
    }
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
    Board::myLevels.push_back(Level::createWithMapObjects(mapObjects, name));
}