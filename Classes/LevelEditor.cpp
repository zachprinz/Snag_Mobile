//
//  LevelEditor.cpp
//  Snag
//
//  Created by Zachary Prinzbach on 7/1/14.
//
//

#include "LevelEditor.h"
#include "MainMenu.h"
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
    
    spikeWallSelectButton = MenuItemImage::create("LevelEditorSpikeWall.png", "LevelEditorSpikewall.png", this, menu_selector(LevelEditor::SpikeWallSelectCallback));
    float buttonHeight = 155 * MainMenu::screenScale.x + 22;
    spikeWallSelectButton->setPosition(Point(10, visibleSize.height - (10 + buttonHeight * 0)));
    spikeWallSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(spikeWallSelectButton);
    
    wallSelectButton = MenuItemImage::create("LevelEditorWall.png", "LevelEditorWall.png", this, menu_selector(LevelEditor::WallSelectCallback));
    wallSelectButton->setPosition(Point(10,visibleSize.height - (10 + buttonHeight * 1)));
    wallSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(wallSelectButton);
    
    spawnerSelectButton = MenuItemImage::create("LevelEditorSpawner.png", "LevelEditorSpawner.png", this, menu_selector(LevelEditor::SpawnerSelectCallback));
    spawnerSelectButton->setPosition(Point(10,visibleSize.height - (10 + buttonHeight * 2)));
    spawnerSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(spawnerSelectButton);
    
    hookSelectButton = MenuItemImage::create("LevelEditorHook.png", "LevelEditorHook.png", this, menu_selector(LevelEditor::HookSelectCallback));
    hookSelectButton->setAnchorPoint(Point(0.0,1.0));
    hookSelectButton->setPosition(Point(10,visibleSize.height - (10 + buttonHeight * 3)));
    menuItems.pushBack(hookSelectButton);
    
    goalSelectButton = MenuItemImage::create("LevelEditorGoal.png", "LevelEditorGoal.png", this, menu_selector(LevelEditor::GoalSelectCallback));
    goalSelectButton->setPosition(Point(10,visibleSize.height - (10 + buttonHeight * 4)));
    goalSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(goalSelectButton);
    
    homeSelectButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelEditor::homeButtonCallback));
    homeSelectButton->setPosition(Point(visibleSize.width  - 10, visibleSize.height - (10 + buttonHeight * 0)));
    homeSelectButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(homeSelectButton);
    
    trashSelectButton = MenuItemImage::create("trash.png", "trash.png", this, menu_selector(LevelEditor::trashButtonCallback));
    trashSelectButton->setPosition(Point(visibleSize.width  - 10,visibleSize.height - (10 + buttonHeight * 2)));
    trashSelectButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(trashSelectButton);
    
    saveSelectButton = MenuItemImage::create("save.png", "save.png", this, menu_selector(LevelEditor::saveButtonCallback));
    saveSelectButton->setPosition(Point(visibleSize.width - 10,visibleSize.height - (10 + buttonHeight * 1)));
    saveSelectButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(saveSelectButton);
    
    eraseSelectButton = MenuItemImage::create("LevelEditorErase.png", "LevelEditorErase.png", this, menu_selector(LevelEditor::EraseSelectCallback));
    eraseSelectButton->setPosition(Point(visibleSize.width - 10,visibleSize.height - (10 + buttonHeight * 3)));
    eraseSelectButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(eraseSelectButton);
    
    moveSelectButton = MenuItemImage::create("LevelEditorMove.png", "LevelEditorMove.png", this, menu_selector(LevelEditor::moveButtonCallback));
    moveSelectButton->setPosition(Point(visibleSize.width - 10,visibleSize.height - (10 + buttonHeight * 4)));
    moveSelectButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(moveSelectButton);
    
    selectedLabel = Label::createWithBMFont("comfortaa_large.fnt", "Pan Tool", TextHAlignment::CENTER);
    selectedLabel->setPosition(visibleSize.width / 2.0, visibleSize.height - 10 - (100 * MainMenu::screenScale.y));
    selectedLabel->setColor(Color3B(0.0,0.0,0.0));
    //selectedLabel->setEnabled(false);
    this->addChild(selectedLabel);
    
    notice = Label::createWithBMFont("comfortaa_large.fnt", "Please add at least one spawner to your level!", TextHAlignment::CENTER);
    notice->setPosition(visibleSize.width / 2.0, visibleSize.height / 2.0 - 1000);
    notice->setScale(0.45);
    notice->setVisible(false);
    notice->setColor(Color3B(0.0,0.0,0.0));
    this->addChild(notice);
    
    saveButtonsOnY = visibleSize.height - (10 + buttonHeight * 4);
    saveButtonsOffY = -100;
    
    shade = Sprite::create("shade.png");
    shade->setAnchorPoint(Point(0,1.0));
    shade->setPosition(Point(0,0));
    shade->setScale(visibleSize.width / shade->getTexture()->getPixelsWide(), visibleSize.height / shade->getTexture()->getPixelsHigh());
    shade->setPositionZ(0);
    
    this->addChild(shade);
    
    cocos2d::Vector<MenuItem*> saveMenuItems;
    
    saveAcceptButton = MenuItemImage::create("check.png", "check.png", this, menu_selector(LevelEditor::saveAcceptCallback));
    saveAcceptButton->setPosition(Point(visibleSize.width / 2.0 + 25,saveButtonsOffY));
    saveAcceptButton->setAnchorPoint(Point(0.0,1.0));
    saveMenuItems.pushBack(saveAcceptButton);
    saveAcceptButton->setPositionZ(3);
    
    saveDeclineButton = MenuItemImage::create("x.png", "x.png", this, menu_selector(LevelEditor::saveDeclineCallback));
    saveDeclineButton->setPosition(Point(visibleSize.width / 2.0 - 25,saveButtonsOffY));
    saveDeclineButton->setAnchorPoint(Point(1.0,1.0));
    saveMenuItems.pushBack(saveDeclineButton);
    saveDeclineButton->setPositionZ(3);
    
    Size editBoxSize = Size(750*MainMenu::screenScale.x, 100 * MainMenu::screenScale.y);
    Scale9Sprite* nameBoxBG = Scale9Sprite::create("line.png");
    nameBoxBG->setContentSize(editBoxSize);
    nameBox = EditBox::create(Size(350,50), nameBoxBG);
    nameBox->setPosition(Point(visibleSize.width / 2.0, visibleSize.height / 2.0 + 100 + 1000));
    nameBox->setFontSize(30);
    nameBox->setMaxLength(18);
    nameBox->setPlaceHolder("level name");
    nameBox->setFontColor(Color3B::WHITE);
    //nameBox->setReturnType(kKeyboardReturnTypeDone);
    //nameBox->setInputMode(kEditBoxInputModeAny);
    //nameBox->setDelegate(this);
    this->addChild(nameBox);
    
    selectedSprite = Sprite::create("LevelEditorSelected.png");
    selectedSprite->setAnchorPoint(Point(0.5,1.0));
    selectedSprite->setPosition(visibleSize.width / 2.0, visibleSize.height - 10);
    
    auto background = Sprite::create("GRID.png");
    background->setAnchorPoint(Point(0,0));
    
    spikeWallSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    wallSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    selectedSprite->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    hookSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    spawnerSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    eraseSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    homeSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    saveSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    trashSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    goalSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    moveSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    //selectedLabel->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    saveAcceptButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    saveDeclineButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);

    
    spikeWallSelectButton->setPositionZ(1);
    wallSelectButton->setPositionZ(1);
    spawnerSelectButton->setPositionZ(1);
    hookSelectButton->setPositionZ(1);
    eraseSelectButton->setPositionZ(1);
    homeSelectButton->setPositionZ(1);

    selectedSprite->setPositionZ(-1);
    background->setPositionZ(-4);
    this->addChild(background);
    this->addChild(selectedSprite);
    
    Menu* menu = Menu::createWithArray(menuItems);
    Menu* menu2 = Menu::createWithArray(saveMenuItems);
    menu2->setAnchorPoint(Point(0.0,0.0));
    menu2->setPosition(0,0);
    menu2->setPositionZ(5);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu2,1);
    this->addChild(menu, 1);
    
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
    switch(currentTool){
        case WALL:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case SPIKE_WALL:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case SPAWNER:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case HOOK:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case NO_TOOL:
            break;
        case ERASE:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case MOVE:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case GOAL:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
    }
}

void LevelEditor::ResetToolPos(){
    switch(currentTool){
        case WALL:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case SPIKE_WALL:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case SPAWNER:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case HOOK:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case NO_TOOL:
            break;
        case ERASE:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case MOVE:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case GOAL:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
    }
}
//    doc.LoadFile(FileUtils::getInstance()->fullPathForFilename("level.xml").c_str());

void LevelEditor::Export(){
    std::string str = FileUtils::getInstance()->getWritablePath() + name + ".xml";
    char* cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    
    Board::myLevels.push_back(str);
    Board::myLevelNames.push_back(name);
    
    tinyxml2::XMLDocument myLevelsDoc;
    myLevelsDoc.LoadFile(FileUtils::getInstance()->fullPathForFilename("MyLevels.xml").c_str());
    tinyxml2::XMLElement* newLevel = myLevelsDoc.NewElement("level");
    newLevel->SetAttribute("name", name.c_str());
    newLevel->SetAttribute("path", str.c_str());
    myLevelsDoc.InsertEndChild(newLevel);
    
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* walls = doc.NewElement("Walls");
    tinyxml2::XMLElement* spawners = doc.NewElement("Spawners");
    tinyxml2::XMLElement* spikewalls = doc.NewElement("SpikeWalls");
    tinyxml2::XMLElement* hooks = doc.NewElement("Hooks");
    tinyxml2::XMLElement* map = doc.NewElement("map");
    tinyxml2::XMLElement* goals = doc.NewElement("Goals");
    map->SetAttribute("name", name.c_str());
    for(int x = 0; x < mapObjects.size(); x++){
        switch(mapObjects[x]->GetType()){
            case WALL:{
                tinyxml2::XMLElement* wall = doc.NewElement("wall");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("width", mapObjects[x]->GetSize().x);
                wall->SetAttribute("height", mapObjects[x]->GetSize().y);
                walls->InsertEndChild(wall);
                break;
            }
            case GOAL:{
                tinyxml2::XMLElement* wall = doc.NewElement("goal");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("width", mapObjects[x]->GetSize().x);
                wall->SetAttribute("height", mapObjects[x]->GetSize().y);
                goals->InsertEndChild(wall);
                break;
            }
            case SPIKE_WALL:{
                tinyxml2::XMLElement* wall = doc.NewElement("spikewall");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("width", mapObjects[x]->GetSize().x);
                wall->SetAttribute("height", mapObjects[x]->GetSize().y);
                spikewalls->InsertEndChild(wall);
                break;
            }
            case SPAWNER:{
                tinyxml2::XMLElement* wall = doc.NewElement("spawner");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                wall->SetAttribute("xVelocity", 100);
                wall->SetAttribute("yVelocity", 600);
                spawners->InsertEndChild(wall);
                break;
            }
            case HOOK:{
                tinyxml2::XMLElement* wall = doc.NewElement("hook");
                wall->SetAttribute("x", mapObjects[x]->GetStart().x);
                wall->SetAttribute("y", mapObjects[x]->GetStart().y);
                hooks->InsertEndChild(wall);
                break;
            }
        }
    }
    map->InsertFirstChild(spawners);
    map->InsertFirstChild(goals);
    map->InsertFirstChild(spikewalls);
    map->InsertFirstChild(walls);
    map->InsertFirstChild(hooks);
    doc.InsertFirstChild(map);

    doc.SaveFile(str.c_str());
    Board::levelPath = cstr;
    Board::customLevel = true;
    std::cout << "FilePath: " << FileUtils::getInstance()->getWritablePath().c_str() << std::endl;
}