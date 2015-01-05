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
#include <map>
#include "NDKHelper/NDKHelper.h"

USING_NS_CC;

Scene* LevelEditor::myScene;
LevelEditor* LevelEditor::Instance;

Scene* LevelEditor::createScene(){
    auto scene = Scene::create();
    auto layer = LevelEditor::create();
    myScene = scene;
    myScene->retain();
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

    verticalAxis = Sprite::create("game_line.png");
    verticalAxis->setAnchorPoint(Vec2(0.5,0.5));
    verticalAxis->setScale(1,1);
    verticalAxis->setScale(MainMenu::screenSize.y / verticalAxis->getBoundingBox().size.height,2);
    verticalAxis->setRotation(90);
    horizontalAxis = Sprite::create("game_line.png");
    horizontalAxis->setAnchorPoint(Vec2(0.5,0.5));
    horizontalAxis->setScale(1,1);
    horizontalAxis->setScale(MainMenu::screenSize.x / horizontalAxis->getBoundingBox().size.width, 2);
    for(int x = -50; x < 50; x++){
        auto tempVertical = Sprite::create("game_line.png");
        tempVertical->setAnchorPoint(Vec2(0.5,0.5));
        tempVertical->setScale(1,1);
        tempVertical->setScale(MainMenu::screenSize.y / tempVertical->getBoundingBox().size.height,0.75);
        tempVertical->setRotation(90);
        verticalGuides.push_back(tempVertical);
        this->addChild(tempVertical,1);
        
        auto tempHorizontal = Sprite::create("game_line.png");
        tempHorizontal->setAnchorPoint(Vec2(0.5,0.5));
        tempHorizontal->setScale(1,1);
        tempHorizontal->setScale(MainMenu::screenSize.x / tempHorizontal->getBoundingBox().size.width, 0.75);
        horizontalGuides.push_back(tempHorizontal);
        this->addChild(tempHorizontal,1);
    }
    this->addChild(verticalAxis,1);
    this->addChild(horizontalAxis,1);
    
    preview = new Preview(Rect(0,0,visibleSize.width,visibleSize.height), this, 0.33);
    preview->editor = true;
    currentSprite = new Scale9Sprite();
    currentSprite->initWithFile("Sliced.png", Rect(0, 0, 60, 60));
    currentSprite->setContentSize(Size(100, 100));
    currentSprite->setPosition(0,0);
    currentSprite->setGlobalZOrder(0);
    currentSprite->setVisible(false);
    currentSprite->setAnchorPoint(Vec2(0,0));
    
    auto listener = EventListenerTouchAllAtOnce::create();
    listener->onTouchesBegan = CC_CALLBACK_2(LevelEditor::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(LevelEditor::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(LevelEditor::onTouchesEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    auto listener2 = EventListenerTouchOneByOne::create();
    listener2->onTouchBegan = CC_CALLBACK_2(LevelEditor::onTouchBegan, this);

    saveDialog = false;
    
    auto background = Sprite::create("GRID2.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Point(0,0));
    background->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setGlobalZOrder(-5);
    this->addChild(background);
    
    std::map<std::string, SEL_MenuHandler> callbacks;
    callbacks["Goal"] = menu_selector(LevelEditor::GoalSelectCallback);
    callbacks["Wall"] = menu_selector(LevelEditor::WallSelectCallback);
    callbacks["Spikewall"] = menu_selector(LevelEditor::SpikeWallSelectCallback);
    callbacks["Spawner"] = menu_selector(LevelEditor::SpawnerSelectCallback);
    callbacks["Hook"] = menu_selector(LevelEditor::HookSelectCallback);
    callbacks["Save"] = menu_selector(LevelEditor::saveButtonCallback);
    callbacks["Play"] = menu_selector(LevelEditor::playButtonCallback);
    callbacks["Home"] = menu_selector(LevelEditor::homeButtonCallback);
    callbacks["Duplicate"] = menu_selector(LevelEditor::duplicateCallback);
    callbacks["Trash"] = menu_selector(LevelEditor::removeCallback);
    cocos2d::Vector<MenuItem*> menuItems;
    elements = MainMenu::LoadElementMap("leveleditor", this, callbacks, &menuItems, this);
    
    elements["Move"]->setVisible(false);
    elements["Velocity"]->setVisible(false);
    elements["Resize"]->setVisible(false);
    elements["Trash"]->setVisible(false);
    elements["Duplicate"]->setVisible(false);
    elements["Move"]->setEnabled(false);
    elements["Velocity"]->setEnabled(false);
    elements["Resize"]->setEnabled(false);
    elements["Trash"]->setEnabled(false);
    elements["Duplicate"]->setEnabled(false);
    elements["Duplicate"]->setAnchorPoint(Vec2(1,0));
    elements["Move"]->setAnchorPoint(Vec2(1,1));
    elements["Resize"]->setAnchorPoint(Vec2(0,0));
    elements["Trash"]->setAnchorPoint(Vec2(0,1));
    elements["Velocity"]->setAnchorPoint(Vec2(0.5,0.5));
    int od = -4;
    elements["Trash"]->setGlobalZOrder(od);
    elements["Move"]->setGlobalZOrder(od);
    elements["Resize"]->setGlobalZOrder(od);
    elements["Duplicate"]->setGlobalZOrder(od);
    elements["Velocity"]->setGlobalZOrder(od);
    hasSelected = false;
    isTransforming = false;
    isRotating = false;
    isScaling = false;
    isTargeting = false;
    elements["Move"]->getEventDispatcher()->setEnabled(false);
    elements["Resize"]->getEventDispatcher()->setEnabled(false);
    elements["Velocity"]->getEventDispatcher()->setEnabled(false);

    float buttonHeightPixels = 155;
    float tempScale = MainMenu::minScreenScale;
    if(tempScale > 1.5)
        tempScale = 1.5;
    float buttonPercent = (buttonHeightPixels * tempScale) / visibleSize.height;
    float leftOverHeight = 1.0 - (5 * buttonPercent);
    float buttonGap = leftOverHeight / 6.0;
    float jump = buttonGap + buttonPercent;
    
    selectedLabel = MainMenu::CreateLabel("Pan Tool", 1);
    selectedLabel->setPosition(elements["Title"]->getBoundingBox().getMidX(), elements["Title"]->getBoundingBox().getMidY());
    selectedLabel->setAnchorPoint(Vec2(0.5,0.5));
    selectedLabel->setScale(0.8);
        
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(selectedLabel,1);
    
    NDKHelper::addSelector("LevelEditor", "finishQuit", CC_CALLBACK_2(LevelEditor::finishQuit, this), this);
    
    std::string buttons[4] = {"Resize", "Velocity", "Move", "Title"};
    for(int x = 0; x < 3; x++){
        this->removeChild(elements[buttons[x]]);
        elements[buttons[x]]->removeFromParent();
        this->addChild(elements[buttons[x]],1);
    }
    
    this->addChild(currentSprite,1);
    
    this->removeChild(elements["Title"]);
    elements["Title"]->removeFromParent();
    this->addChild(elements["Title"],1);
    
    this->removeChild(selectedLabel);
    selectedLabel->removeFromParent();
    this->addChild(selectedLabel,1);
    
    savePopUp = new PopUp("Save", "Please name your level.", this, menu_selector(LevelEditor::saveAcceptCallback), menu_selector(LevelEditor::saveDeclineCallback),true);
    savePopUp->Add(this);
    
    spawnerPopUp = new PopUp("Error", "Please add at least one\nspawner to your map.", this, menu_selector(LevelEditor::closePopUpCallback));
    spawnerPopUp->Add(this);
    
    hookPopUp = new PopUp("Error", "Please add at least one\nhook to your map.", this, menu_selector(LevelEditor::closePopUpCallback));
    hookPopUp->Add(this);
    
    quitPopUp = new PopUp("Warning: Unsaved Changes", "Unsaved changes will be lost.\nDo you want to exit?", this, menu_selector(LevelEditor::acceptQuitCallback), menu_selector(LevelEditor::declineQuitCallback));
    quitPopUp->Add(this);
    
    buttonsOn = true;
    currentSelection = NULL;
    currentTool = NO_TOOL;
    UpdateAxis();
    return true;
}
void LevelEditor::RemoveButtons(bool on){
    std::string buttons[8] = {"Wall", "Spikewall", "Hook", "Spawner", "Goal", "Home", "Play", "Save"};
    if(on && !buttonsOn)
        return;
    if(!on && buttonsOn)
        return;
    if(on)
        buttonsOn = false;
    if(!on)
        buttonsOn = true;
    for(int x = 0; x < 8; x++){
        float moveDistance = 2*elements[buttons[x]]->getBoundingBox().size.width;
        auto setAction = MoveTo::create(0.2, Point(elements[buttons[x]]->getPosition().x + moveDistance, elements[buttons[x]]->getPosition().y));
        auto setActionNegative = MoveTo::create(0.2, Point(elements[buttons[x]]->getPosition().x - moveDistance, elements[buttons[x]]->getPosition().y));
        if(on){
            if(x <= 4)
                elements[buttons[x]]->runAction(setActionNegative);
            else
                elements[buttons[x]]->runAction(setAction);
        } else{
            if(x <= 4)
                elements[buttons[x]]->runAction(setAction);
            else
                elements[buttons[x]]->runAction(setActionNegative);
        }
    }
}
void LevelEditor::transformCallback(){
    isTransforming = true;
};
void LevelEditor::resizeCallback(){
    isScaling = true;
};
void LevelEditor::removeCallback(Ref*){
    Deselect(false);
};
void LevelEditor::duplicateCallback(Ref*){
    Entity* tempEnt = new Entity(Vec2(selectedEntity->GetPosition().x + 100, selectedEntity->GetPosition().y + 100), selectedEntity->GetSize(), selectedEntity->GetLaunchVelocity(), selectedEntity->GetType());
    entities[tempEnt->ID] = tempEnt;
    preview->AddEntity(tempEnt);
};
void LevelEditor::targetCallback(){
    isTargeting = true;
}
void LevelEditor::acceptQuitCallback(Ref*){
    if(currentLevel->GetName().compare("qq36q81q") == 0){
        ValueMap valueMap;
        valueMap["id"] = currentLevel->GetID();
        valueMap["refresh"] = 0;
        Value parameters = Value(valueMap);
        sendMessageWithParams("deleteLevel", parameters);
    } else {
        Clear();
        quitPopUp->Close();
        goToHome();
    }
}
void LevelEditor::declineQuitCallback(Ref*){
    quitPopUp->Close();
}
void LevelEditor::finishQuit(Node* sender, Value data){
    Clear();
    quitPopUp->Close();
    goToHome();
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
    if(currentSelection != elements["Spikewall"]){
        ResetToolPos();
        currentTool = SPIKE_WALL;
        currentSelection = elements["Spikewall"];
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
    if(currentSelection != elements["Wall"]){
        ResetToolPos();
        currentTool = WALL;
        currentSelection = elements["Wall"];
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
    if(currentSelection != elements["Spawner"]){
        ResetToolPos();
        currentTool = SPAWNER;
        currentSelection = elements["Spawner"];
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
    if(currentSelection != elements["Hook"]){
        ResetToolPos();
        currentSelection = elements["Hook"];
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
    if(currentSelection != elements["Trash"]){
        ResetToolPos();
        currentSelection = elements["Trash"];
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
    if(unsavedChanges || currentLevel->GetName().compare("qq36q81q") == 0){
        quitPopUp->Show();
    } else {
        goToHome();
    }
}
void LevelEditor::goToHome(){
    if(LevelSelect::myScene == NULL){
        auto scene = LevelSelect::createScene();
        LevelSelect::Instance->Refresh();
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(transition);
    }
    else{
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(transition);
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
    //Taken from export, doesn't save to server but updates Level object.
}
void LevelEditor::saveAcceptCallback(Ref* ref){
    name = savePopUp->GetText();
    Export();
    saveDialog = false;
    savePopUp->Close();
    unsavedChanges = false;
}
void LevelEditor::saveDeclineCallback(Ref* ref){
    saveDialog = false;
    savePopUp->Close();
}
void LevelEditor::GoalSelectCallback(Ref* ref){
    if(currentSelection != elements["Goal"]){
        ResetToolPos();
        currentSelection = elements["Goal"];
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
    touchCurrent = Vec2(0,0);
    touchStart = Vec2(0,0);
    currentTouches.clear();
    ResetToolPos();
    currentTool = NO_TOOL;
    currentSelection = NULL;
    selectedLabel->setString("Pan Tool");
    SetToolPos();
    preview->Reset();
    EnableSpawner();
    Deselect();
}
bool LevelEditor::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    unsavedChanges = true;
    if(noticeUp){
        notice->setVisible(false);
        notice->setPosition(Point(notice->getPosition().x, notice->getPosition().y - 1000));
        noticeUp = false;
    }
    else{
        if(!saveDialog){
            touchStart = touch->getLocation();
            touchCurrent = touch->getLocation();
            bool hitTab = false;
            if(elements["Move"]->isVisible() && elements["Move"]->getBoundingBox().containsPoint(touchStart)){
               transformCallback();
                touchDifference.x = currentSprite->getPosition().x - touchStart.x;
                touchDifference.y = currentSprite->getPosition().y - touchStart.y;
                hitTab = true;
                return true;
            }
            if(elements["Resize"]->isVisible() && elements["Resize"]->getBoundingBox().containsPoint(touchStart)){
                resizeCallback();
                touchDifference.x = touchStart.x - currentSprite->getBoundingBox().getMaxX();
                touchDifference.y = touchStart.y - currentSprite->getBoundingBox().getMaxY();
                hitTab = true;
                return true;
            }
            if(elements["Velocity"]->isVisible() && elements["Velocity"]->getBoundingBox().containsPoint(touchStart)){
                targetCallback();
                hitTab = true;
                return true;
            }
            //Check if the user selected an entity//
            if(hitTab == false){
                if(hasSelected){
                    Deselect();
                    return false;
                }
                Entity* target = preview->GetTarget(touchStart);
                if(target != nullptr){
                    Select(target);
                    return false;
                }
            }
            if(currentTool == HOOK || currentTool == SPAWNER){
                touchStart = preview->MapToScreen(CheckSnap(preview->ScreenToMap(touchStart)));
                touchCurrent = touchStart;
                Entity* tempEnt = (preview->CreateEntity(touchStart, touchStart, currentTool));
                entities[tempEnt->ID] = tempEnt;
                preview->AddEntity(tempEnt);
                UnselectTool();
                return false;
            } else {
                if(currentTool == NO_TOOL){
                    if(hasSelected)
                        Deselect();
                } else {
                    Vec2 screenToMap = preview->ScreenToMap(touchStart);
                    Vec2 snapped = CheckSnap(screenToMap);
                    Vec2 snappedScreen = preview->MapToScreen(snapped);
                    touchStart = snappedScreen;
                    currentSprite->setPosition(touchStart);
                    printf("\n Current Sprite Pos: (%f, %f)\n", touchStart.x, touchStart.y);
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
void LevelEditor::Select(Entity* entity){
    if(hasSelected)
        Deselect();
    hasSelected = true;
    currentSprite->setVisible(true);
    selectedEntity = entity;
    Vec2 startPosition = preview->MapToScreen(selectedEntity->GetPosition());
    Size selectedEntitySize = Size(preview->GetScale() * entity->GetSize().x, preview->GetScale() * selectedEntity->GetSize().y);
    startPosition.x -= selectedEntitySize.width / 2.0;
    startPosition.y -= selectedEntitySize.height / 2.0;
    currentSprite->setPosition(startPosition);
    currentSprite->setContentSize(selectedEntitySize);
    elements["Duplicate"]->setEnabled(true);
    elements["Resize"]->setEnabled(true);
    elements["Move"]->setEnabled(true);
    elements["Trash"]->setEnabled(true);
    elements["Duplicate"]->setVisible(true);
    elements["Resize"]->setVisible(true);
    elements["Move"]->setVisible(true);
    elements["Trash"]->setVisible(true);
    Rect box = currentSprite->getBoundingBox();
    elements["Duplicate"]->setPosition(Vec2(box.getMinX(), box.getMaxY()));
    elements["Resize"]->setPosition(Vec2(box.getMaxX(), box.getMaxY()));
    elements["Move"]->setPosition(Vec2(box.getMinX(), box.getMinY()));
    elements["Trash"]->setPosition(Vec2(box.getMaxX(), box.getMinY()));
    if(selectedEntity->GetType() == SPAWNER){
        elements["Velocity"]->setVisible(true);
        Vec2 launchVel = selectedEntity->GetLaunchVelocity();
        //launchVel = Vec2(launchVel.x / 2.0, launchVel.x / 2.0);
        Vec2 targetOffset = Vec2(launchVel.x * preview->GetScale(), launchVel.y * preview->GetScale());
        Vec2 tempPos = (Vec2(box.getMidX() + targetOffset.x, box.getMidY() + targetOffset.y));
        elements["Velocity"]->setPosition(tempPos);
        float angle = atan2(targetOffset.x, targetOffset.y) * (180 / 3.1415);
        elements["Velocity"]->setRotation(angle);
    }
    RemoveButtons(true);
}
void LevelEditor::Deselect(bool keep){
    if(keep && selectedEntity != nullptr){
        preview->AddEntity(selectedEntity);
        preview->Update();
    }
    printf("\nDESELECTED");
    hasSelected = false;
    selectedEntity = nullptr;
    elements["Move"]->setEnabled(false);
    elements["Duplicate"]->setEnabled(false);
    elements["Trash"]->setEnabled(false);
    elements["Resize"]->setEnabled(false);
    currentSprite->setVisible(false);
    elements["Velocity"]->setVisible(false);
    elements["Duplicate"]->setVisible(false);
    elements["Resize"]->setVisible(false);
    elements["Move"]->setVisible(false);
    elements["Trash"]->setVisible(false);
    isRotating = false;
    isTransforming = false;
    isScaling = false;
    isTargeting = false;
    RemoveButtons(false);
}
void LevelEditor::UpdateSelected(){
    if(hasSelected){
        Vec2 startPosition = preview->MapToScreen(selectedEntity->GetPosition());
        Size selectedEntitySize = Size(preview->GetScale() * selectedEntity->GetSize().x, preview->GetScale() * selectedEntity->GetSize().y);
        startPosition.x -= selectedEntitySize.width / 2.0;
        startPosition.y -= selectedEntitySize.height / 2.0;
        currentSprite->setPosition(startPosition);
        currentSprite->setContentSize(selectedEntitySize);
        Rect box = currentSprite->getBoundingBox();
        elements["Duplicate"]->setPosition(Vec2(box.getMinX(), box.getMaxY()));
        elements["Resize"]->setPosition(Vec2(box.getMaxX(), box.getMaxY()));
        elements["Move"]->setPosition(Vec2(box.getMinX(), box.getMinY()));
        elements["Trash"]->setPosition(Vec2(box.getMaxX(), box.getMinY()));
        if(selectedEntity->GetType() == SPAWNER){
            Vec2 launchVel = selectedEntity->GetLaunchVelocity();
            Vec2 targetOffset = Vec2(launchVel.x * preview->GetScale(), launchVel.y * preview->GetScale());
            Vec2 tempPos = (Vec2(box.getMidX() + targetOffset.x, box.getMidY() + targetOffset.y));
            elements["Velocity"]->setPosition(tempPos);
            float angle = atan2(targetOffset.x, targetOffset.y) * (180 / 3.1415);
            elements["Velocity"]->setRotation(angle);
        }
    }
}
void LevelEditor::UpdateAxis(){
    Vec2 originPos = preview->MapToScreen(Vec2(0,0));
    verticalAxis->setPosition(originPos.x, MainMenu::screenSize.y / 2.0);
    horizontalAxis->setPosition(MainMenu::screenSize.x / 2.0, originPos.y);
    horizontalAxis->setScaleY(3);
    verticalAxis->setScaleY(3);
    for(int x = 0; x < 100; x++){
        Vec2 tempPos = preview->MapToScreen(Vec2((PTM_RATIO/4.0) * (-20 + x), (PTM_RATIO/4.0) * (-20 + x)));
        verticalGuides[x]->setPosition(tempPos.x, verticalGuides[x]->getPosition().y);
        horizontalGuides[x]->setPosition(MainMenu::screenSize.x / 2.0, tempPos.y);
        float tempScale = preview->GetScale() * 1;
        if(tempScale > 1)
            tempScale = 1;
        if(tempScale < 0.5)
            tempScale = 0.5;
        if(x%4 == 0)
            tempScale *= 2;
        horizontalGuides[x]->setScaleY(tempScale);
        verticalGuides[x]->setScaleY(tempScale);
    }
}
void LevelEditor::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    if(isScaling || isRotating || isTransforming || isTargeting){
        isScaling = false;
        isRotating = false;
        isTransforming = false;
        isTargeting = false;
    } else {
        if(!hasSelected && currentTool != ERASE && currentTool != NO_TOOL){
            if(currentTool != SPAWNER && currentTool != HOOK){
                if(false){//std::abs(currentSprite->getContentSize().width) < (0.5 * TILE_SIZE * (1.0/preview->GetScale())) || std::abs(currentSprite->getContentSize().height) < (0.5 * (TILE_SIZE*(1.0/preview->GetScale())))){
                    currentSprite->setVisible(false);
                } else{
                    Rect bb = currentSprite->getBoundingBox();
                    Vec2 minPoint(bb.getMinX(), bb.getMinY());
                    Vec2 maxPoint(bb.getMaxX(), bb.getMaxY());
                    Entity* tempEnt = (preview->CreateEntity(minPoint , maxPoint, currentTool));
                    entities[tempEnt->ID] = tempEnt;
                    preview->AddEntity(tempEnt);
                    currentSprite->setVisible(false);
                    preview->Update();
                }
            }
            UnselectTool();
        }
        if(currentTool == ERASE){
            Entity* entToRemove = preview->GetTarget(currentTouches[0]->getLocation());
            preview->RemoveEntity(entToRemove);
            entities.erase(entToRemove->ID);
        }
    }
}
void LevelEditor::onTouchMoved(Touch* touch, Event* event){
    if(isRotating || isScaling || isTransforming || isTargeting){
        Vec2 oldTouch = touchCurrent;
        touchCurrent = touch->getLocation();
        if(isScaling){
            if(currentSprite != NULL){
                Vec2 touchCurrentMod(touchCurrent.x - touchDifference.x, touchCurrent.y - touchDifference.y);
                touchCurrent = preview->MapToScreen(CheckSnap(preview->ScreenToMap(touchCurrentMod)));
                touchCurrent.x += touchDifference.x;
                touchCurrent.y += touchDifference.y;
                Vec2 tempCurrentSize = currentSprite->getContentSize();
                currentSprite->setContentSize(Size(currentSprite->getContentSize().width + 0.5*(touchCurrent.x - oldTouch.x), currentSprite->getContentSize().height + 0.5*(touchCurrent.y - oldTouch.y)));
                if(currentSprite->getContentSize().width < 100 * preview->GetScale())
                    currentSprite->setContentSize(Size(tempCurrentSize.x, currentSprite->getContentSize().height));
                if(currentSprite->getContentSize().height < 100 * preview->GetScale())
                    currentSprite->setContentSize(Size(currentSprite->getContentSize().width, tempCurrentSize.y));
                Vec2 endResult(currentSprite->getContentSize().width - tempCurrentSize.x, currentSprite->getContentSize().height - tempCurrentSize.y);
                currentSprite->setPosition(currentSprite->getPosition().x + endResult.x, currentSprite->getPosition().y + endResult.y);
                selectedEntity->SetPosition(Vec2(selectedEntity->GetPosition().x + 0.5*(endResult.x/preview->GetScale()), selectedEntity->GetPosition().y + 0.5*(endResult.y / preview->GetScale())));
                selectedEntity->SetSize(Vec2(currentSprite->getContentSize().width / preview->GetScale(), currentSprite->getContentSize().height / preview->GetScale()));
                this->UpdateSelected();
            }
        }
        if(isTransforming){
            if(currentSprite != NULL){
                Vec2 touchCurrentMod(touchCurrent.x + touchDifference.x, touchCurrent.y + touchDifference.y);
                touchCurrent = preview->MapToScreen(CheckSnap(preview->ScreenToMap(touchCurrentMod)));
                touchCurrent.x -= touchDifference.x;
                touchCurrent.y -= touchDifference.y;
                currentSprite->setPosition(Vec2(currentSprite->getPosition().x + (touchCurrent.x - oldTouch.x), currentSprite->getPosition().y + (touchCurrent.y - oldTouch.y)));
                selectedEntity->SetPosition(preview->ScreenToMap(Vec2(currentSprite->getBoundingBox().getMidX(), currentSprite->getBoundingBox().getMidY())));
                this->UpdateSelected();
            }
        }
        if(isRotating){
            
        }
        if(isTargeting){
            if(currentSprite != NULL){
                elements["Velocity"]->setPosition(Vec2(elements["Velocity"]->getPosition().x + (touchCurrent.x - oldTouch.x), elements["Velocity"]->getPosition().y + (touchCurrent.y - oldTouch.y)));
                selectedEntity->SetLaunchVelocity(Vec2((elements["Velocity"]->getBoundingBox().getMidX() - currentSprite->getBoundingBox().getMidX())/preview->GetScale(), (elements["Velocity"]->getBoundingBox().getMidY() - currentSprite->getBoundingBox().getMidY())/preview->GetScale()));
                this->UpdateSelected();
            }
        }
    } else {
        if(currentTool != NO_TOOL && currentTool != ERASE){
            touchCurrent = touch->getLocation();
            Vec2 touchCurrent2;
            if(currentSprite != NULL)
                touchCurrent2 = preview->MapToScreen(CheckSnap(preview->ScreenToMap(touchCurrent)));
                currentSprite->setContentSize(Size(touchCurrent2.x - touchStart.x, touchCurrent2.y - touchStart.y));
        }
        if(currentTool == NO_TOOL){
            Vec2 oldTouch = touchStart;
            touchCurrent = touch->getLocation();
            touchStart  = touchCurrent;
            preview->Drag(touchCurrent - oldTouch);
        }
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
    preview->Update();
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
    if(currentSelection == elements["Spawner"]){
        elements["Spawner"]->setEnabled(false);
        ResetToolPos();
        currentTool = NO_TOOL;
        currentSelection = NULL;
    }
}
void LevelEditor::EnableSpawner(){
    elements["Spawner"]->setEnabled(true);
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
Vec2 LevelEditor::CheckSnap(Vec2 touchMap){
    float snapRatio = 0.33;
    printf("\nMap Pos Touch: (%f, %f)\n", touchMap.x, touchMap.y);
    if(fmod(touchMap.x,PTM_RATIO / 4.0) < (PTM_RATIO * 0.25 * snapRatio)){
        touchMap.x -= fmod(touchMap.x, PTM_RATIO / 4.0);
    }
    if(fmod(touchMap.x, PTM_RATIO / 4.0) > (PTM_RATIO * 0.25 * (1.0 - snapRatio))){
        touchMap.x += PTM_RATIO / 4.0 - (fmod(touchMap.x, PTM_RATIO / 4.0));
    }
    if(fmod(touchMap.y,PTM_RATIO / 4.0) < (PTM_RATIO * 0.25 * snapRatio)){
        touchMap.y -= fmod(touchMap.y, PTM_RATIO / 4.0);
    }
    if(fmod(touchMap.y, PTM_RATIO / 4.0) > (PTM_RATIO * 0.25 * (1.0 - snapRatio))){
        touchMap.y += PTM_RATIO / 4.0 - (fmod(touchMap.y, PTM_RATIO / 4.0));
    }
    return touchMap;
}
void LevelEditor::UnselectTool(){
    ResetToolPos();
    currentTool = NO_TOOL;
    currentSelection = NULL;
    selectedLabel->setString("Pan Tool");
}
void LevelEditor::Export(){
    currentLevel->SetName(name);
    currentLevel->Clear();
    for (std::map<int,Entity*>::iterator it=entities.begin(); it!=entities.end(); ++it){
        int entID = ((Entity*)it->second)->ID;
        currentLevel->AddEntity(entities[entID]);
    }
    currentLevel->Save();
    unsavedChanges = false;
}