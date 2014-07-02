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

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(LevelEditor::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(LevelEditor::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(LevelEditor::onTouchMoved, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    cocos2d::Vector<MenuItem*> menuItems;
    
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
    
    eraseSelectButton = MenuItemImage::create("LevelEditorErase.png", "LevelEditorErase.png", this, menu_selector(LevelEditor::EraseSelectCallback));
    eraseSelectButton->setPosition(Point(10,visibleSize.height - (10 + buttonHeight * 4)));
    eraseSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(eraseSelectButton);
    
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelEditor::homeButtonCallback));
    homeButton->setPosition(Point(visibleSize.width  - 10, visibleSize.height - (10 + buttonHeight * 0)));
    homeButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(homeButton);
    
    auto trashButton = MenuItemImage::create("trash.png", "trash.png", this, menu_selector(LevelEditor::trashButtonCallback));
    trashButton->setPosition(Point(visibleSize.width  - 10,visibleSize.height - (10 + buttonHeight * 2)));
    trashButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(trashButton);
    
    auto saveButton = MenuItemImage::create("save.png", "save.png", this, menu_selector(LevelEditor::saveButtonCallback));
    saveButton->setPosition(Point(visibleSize.width - 10,visibleSize.height - (10 + buttonHeight * 1)));
    saveButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(saveButton);
    
    auto playButton = MenuItemImage::create("LevelEditorPlay.png", "LevelEditorPlay.png", this, menu_selector(LevelEditor::playButtonCallback));
    playButton->setPosition(Point(visibleSize.width - 10,visibleSize.height - (10 + buttonHeight * 3)));
    playButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(playButton);
    
    moveSelectButton = MenuItemImage::create("LevelEditorMove.png", "LevelEditorMove.png", this, menu_selector(LevelEditor::moveButtonCallback));
    moveSelectButton->setPosition(Point(visibleSize.width - 10,visibleSize.height - (10 + buttonHeight * 4)));
    moveSelectButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(moveSelectButton);
    
    selectedLabel = MenuItemFont::create("New Map 1", this, menu_selector(LevelEditor::selectedButtonCallback));
    selectedLabel->setAnchorPoint(Point(0.5,1.15));
    selectedLabel->setPosition(visibleSize.width / 2.0, visibleSize.height - 10);
    selectedLabel->setFontSizeObj(selectedLabel->getFontSize() * 5);
    selectedLabel->setColor(Color3B(0.0,0.0,0.0));
    selectedLabel->setEnabled(false);
    menuItems.pushBack(selectedLabel);
    
    
    auto buttonsBackground = Sprite::create("LevelEditorSelected.png");
    buttonsBackground->setAnchorPoint(Point(0.5,1.0));
    buttonsBackground->setPosition(visibleSize.width / 2.0, visibleSize.height - 10);
    
    auto background = Sprite::create("GRID.png");
    background->setAnchorPoint(Point(0,0));
    
    spikeWallSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    wallSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    buttonsBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    hookSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    spawnerSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    eraseSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    homeButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    saveButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    trashButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    playButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    moveSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    selectedLabel->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    
    spikeWallSelectButton->setPositionZ(2);
    wallSelectButton->setPositionZ(2);
    spawnerSelectButton->setPositionZ(2);
    hookSelectButton->setPositionZ(2);
    eraseSelectButton->setPositionZ(2);
    homeButton->setPositionZ(2);

    buttonsBackground->setPositionZ(-1);
    background->setPositionZ(-4);
    this->addChild(background);
    this->addChild(buttonsBackground);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    currentSelection = NULL;
    
    return true;
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
    }
};
void LevelEditor::homeButtonCallback(Ref* ref){
    Director::getInstance()->pushScene(MainMenu::myScene);
}
void LevelEditor::saveButtonCallback(Ref* ref){
    Export();
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
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
        case MOVE:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
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
                currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
            }
            break;
        }
        case MOVE:{
            if(currentSelection != NULL){
                currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
            }
            break;
        }
    }
}
//    doc.LoadFile(FileUtils::getInstance()->fullPathForFilename("level.xml").c_str());

void LevelEditor::Export(){
    tinyxml2::XMLDocument doc;
    tinyxml2::XMLElement* walls = doc.NewElement("Walls");
    tinyxml2::XMLElement* spawners = doc.NewElement("Spawners");
    tinyxml2::XMLElement* spikewalls = doc.NewElement("SpikeWalls");
    tinyxml2::XMLElement* hooks = doc.NewElement("Hooks");
    tinyxml2::XMLElement* map = doc.NewElement("map");
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
    map->InsertFirstChild(spikewalls);
    map->InsertFirstChild(walls);
    map->InsertFirstChild(hooks);
    doc.InsertFirstChild(map);
    std::string str = FileUtils::getInstance()->getWritablePath() + "customlevel.xml";
    char* cstr = new char[str.length() + 1];
    std::strcpy(cstr, str.c_str());
    doc.SaveFile(cstr);
    Board::levelPath = cstr;
    std::cout << "FilePath: " << FileUtils::getInstance()->getWritablePath().c_str() << std::endl;
}