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
    float buttonHeight = 108 * MainMenu::screenScale.x;
    spikeWallSelectButton->setPosition(Point(22, visibleSize.height - (25 + buttonHeight * 0)));
    spikeWallSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(spikeWallSelectButton);
    
    wallSelectButton = MenuItemImage::create("LevelEditorWall.png", "LevelEditorWall.png", this, menu_selector(LevelEditor::WallSelectCallback));
    wallSelectButton->setPosition(Point(22,visibleSize.height - (25 + buttonHeight * 1)));
    wallSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(wallSelectButton);
    
    spawnerSelectButton = MenuItemImage::create("LevelEditorSpawner.png", "LevelEditorSpawner.png", this, menu_selector(LevelEditor::SpawnerSelectCallback));
    spawnerSelectButton->setPosition(Point(22,visibleSize.height - (25 + buttonHeight * 2)));
    spawnerSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(spawnerSelectButton);
    
    hookSelectButton = MenuItemImage::create("LevelEditorHook.png", "LevelEditorHook.png", this, menu_selector(LevelEditor::HookSelectCallback));
    hookSelectButton->setAnchorPoint(Point(0.0,1.0));
    hookSelectButton->setPosition(Point(22,visibleSize.height - (25 + buttonHeight * 3)));
    menuItems.pushBack(hookSelectButton);
    
    eraseSelectButton = MenuItemImage::create("LevelEditorErase.png", "LevelEditorErase.png", this, menu_selector(LevelEditor::EraseSelectCallback));
    eraseSelectButton->setPosition(Point(22,visibleSize.height - (28 + buttonHeight * 4)));
    eraseSelectButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(eraseSelectButton);
    
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(LevelEditor::homeButtonCallback));
    homeButton->setPosition(Point(visibleSize.width  - 15, visibleSize.height -15));//52 + ((visibleSize.width - 100) / 4.0) * 3,- 100));
    homeButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(homeButton);
    
    auto buttonsBackground = Sprite::create("LevelEditorBackground.png");
    buttonsBackground->setAnchorPoint(Point(0.0,1.0));
    buttonsBackground->setPosition(15, visibleSize.height - 15);
    
    auto background = Sprite::create("GRID.png");
    background->setAnchorPoint(Point(0,0));
    
    spikeWallSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    wallSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    buttonsBackground->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    hookSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    spawnerSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    eraseSelectButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    
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
        if(currentSelection != NULL){
            currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        }
        currentTool = SPIKE_WALL;
        currentSelection = spikeWallSelectButton;
        currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
    }
    else{
        currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
};
void LevelEditor::WallSelectCallback(Ref*){
    if(currentSelection != wallSelectButton){
        if(currentSelection != NULL){
            currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        }
        currentTool = WALL;
        currentSelection = wallSelectButton;
        currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
    }
    else{
        currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
};
void LevelEditor::SpawnerSelectCallback(Ref*){
    Instance = this;
    if(currentSelection != spawnerSelectButton){
        if(currentSelection != NULL){
            currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        }
        currentTool = SPAWNER;
        currentSelection = spawnerSelectButton;
        currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
    }
    else{
        currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
};
void LevelEditor::HookSelectCallback(Ref*){
    if(currentSelection != hookSelectButton){
        if(currentSelection != NULL){
            currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        }
        currentSelection = hookSelectButton;
        currentTool = HOOK;
        currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
    }
    else{
        currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
};
void LevelEditor::EraseSelectCallback(Ref*){
    if(currentSelection != eraseSelectButton){
        if(currentSelection != NULL){
            currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        }
        currentSelection = eraseSelectButton;
        currentTool = ERASE;
        currentSelection->setPosition(currentSelection->getPosition().x + 20, currentSelection->getPosition().y);
    }
    else{
        currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
};
void LevelEditor::homeButtonCallback(Ref* ref){
    Director::getInstance()->pushScene(MainMenu::myScene);
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
            break;
    }
    return true;
}
void LevelEditor::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    
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
    Vec2 ret = Vec2(ceil((float)pos.x / TILE_SIZE), ceil((float)pos.y / TILE_SIZE));
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
        currentSelection->setPosition(currentSelection->getPosition().x - 20, currentSelection->getPosition().y);
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
}
void LevelEditor::EnableSpawner(){
    spawnerSelectButton->setEnabled(true);
}
