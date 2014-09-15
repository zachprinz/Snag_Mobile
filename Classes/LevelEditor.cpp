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

    verticalAxis = MainMenu::CreateButton("line.png", Vec2(0.5,0.5), Vec2(0,0));
    verticalAxis->setAnchorPoint(Vec2(0.5,0.5));
    verticalAxis->setScale(1,1);
    verticalAxis->setScale(MainMenu::screenSize.y / verticalAxis->getBoundingBox().size.height,2);
    verticalAxis->setRotation(90);
    horizontalAxis = MainMenu::CreateButton("line.png", Vec2(0.5,0.5), Vec2(0,0));
    horizontalAxis->setAnchorPoint(Vec2(0.5,0.5));
    horizontalAxis->setScale(1,1);
    horizontalAxis->setScale(MainMenu::screenSize.x / horizontalAxis->getBoundingBox().size.width, 2);
    horizontalAxis->setGlobalZOrder(-4);
    verticalAxis->setGlobalZOrder(-4);
    for(int x = -50; x < 50; x++){
        auto tempVertical = MainMenu::CreateButton("line.png", Vec2(0.5,0.5), Vec2(0,0));
        tempVertical->setAnchorPoint(Vec2(0.5,0.5));
        tempVertical->setScale(1,1);
        tempVertical->setScale(MainMenu::screenSize.y / tempVertical->getBoundingBox().size.height,0.75);
        tempVertical->setRotation(90);
        verticalGuides.push_back(tempVertical);
        this->addChild(tempVertical,1);
        
        auto tempHorizontal = MainMenu::CreateButton("line.png", Vec2(0.5,0.5), Vec2(0,0));
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
    //listener->setSwallowTouches(true);
    listener->onTouchesBegan = CC_CALLBACK_2(LevelEditor::onTouchesBegan, this);
    listener->onTouchesMoved = CC_CALLBACK_2(LevelEditor::onTouchesMoved, this);
    listener->onTouchesEnded = CC_CALLBACK_2(LevelEditor::onTouchesEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    cocos2d::Vector<MenuItem*> menuItems;
    saveDialog = false;
    
    selectedSprite = MainMenu::CreateButton("LevelEditorSelected.png", Vec2(0,1.0-0.015), Vec2(0,1));
    selectedSprite->setScale(0.33);
    selectedSprite->setPosition(visibleSize.width / 2.0, selectedSprite->getPosition().y);
    selectedSprite->setAnchorPoint(Point(0.5,1.0));
    auto background = Sprite::create("GRID2.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Point(0,0));
    background->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    background->setGlobalZOrder(-5);
    this->addChild(background);
    menuItems.pushBack(selectedSprite);
    
    transform = MainMenu::CreateButton("Transform.png", Vec2(0,0), Vec2(0,0));//this, menu_selector(LevelEditor::transformCallback), Vec2(0,0), Vec2(0,0));
    menuItems.pushBack(transform);
    resize = MainMenu::CreateButton("Resize.png", Vec2(0,0), Vec2(0,0));
    menuItems.pushBack(resize);
    remove = MainMenu::CreateButton("Remove.png", this, menu_selector(LevelEditor::removeCallback), Vec2(0,0), Vec2(0,0));
    menuItems.pushBack(remove);
    rotate = MainMenu::CreateButton("Rotate.png", Vec2(0,0), Vec2(0,0));
    menuItems.pushBack(rotate);
    //rotate->setDisabledImage(Sprite::create("RotateDisabled.png"));
    //remove->setDisabledImage(Sprite::create("RemoveDisabled.png"));
    //transform->setDisabledImage(Sprite::create("TransformDisabled.png"));
    //resize->setDisabledImage(Sprite::create("ResizeDisabled.png"));
    //transform->setRotation(180.f);
    //resize->setRotation(180.f);
    transform->setVisible(false);
    resize->setVisible(false);
    remove->setVisible(false);
    rotate->setVisible(false);
    rotate->setAnchorPoint(Vec2(1,0));
    transform->setAnchorPoint(Vec2(1,1));
    resize->setAnchorPoint(Vec2(0,0));
    remove->setAnchorPoint(Vec2(0,1));
    int od = -4;
    remove->setGlobalZOrder(od);
    transform->setGlobalZOrder(od);
    resize->setGlobalZOrder(od);
    rotate->setGlobalZOrder(od);
    hasSelected = false;
    isTransforming = false;
    isRotating = false;
    isScaling = false;
    transform->getEventDispatcher()->setEnabled(false);
    resize->getEventDispatcher()->setEnabled(false);
    rotate->getEventDispatcher()->setEnabled(false);

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
    saveSelectButton = MainMenu::CreateButton("save.png", this, menu_selector(LevelEditor::saveButtonCallback), Vec2(0.9,1.0-buttonGap-(4 * jump)), Vec2(1,0));
    menuItems.pushBack(saveSelectButton);
    eraseSelectButton = MainMenu::CreateButton("LevelEditorErase.png", this, menu_selector(LevelEditor::EraseSelectCallback), Vec2(0.9,1.0-buttonGap-(3*jump)), Vec2(1,0));
    moveSelectButton = MainMenu::CreateButton("LevelEditorMove.png", this, menu_selector(LevelEditor::moveButtonCallback), Vec2(0.9,1.0-buttonGap-(4*jump)), Vec2(1,0));
    
    selectedLabel = MainMenu::CreateLabel("Pan Tool", Vec2(0,1.0-0.015), Vec2(0,0));
    selectedLabel->setGlobalZOrder(0);
    selectedLabel->setPosition(selectedSprite->getBoundingBox().getMidX(), selectedSprite->getBoundingBox().getMidY());
    selectedLabel->setAnchorPoint(Vec2(0.5,0.5));
    selectedLabel->setScale(0.8);
        
    menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    this->addChild(selectedLabel,1);
    
    NDKHelper::addSelector("LevelEditor", "finishQuit", CC_CALLBACK_2(LevelEditor::finishQuit, this), this);
    
    savePopUp = new PopUp("Save", "Please name your level.", this, menu_selector(LevelEditor::saveAcceptCallback), menu_selector(LevelEditor::saveDeclineCallback),true);
    savePopUp->Add(this);
    
    spawnerPopUp = new PopUp("Error", "Please add at least one\nspawner to your map.", this, menu_selector(LevelEditor::closePopUpCallback));
    spawnerPopUp->Add(this);
    
    hookPopUp = new PopUp("Error", "Please add at least one\nhook to your map.", this, menu_selector(LevelEditor::closePopUpCallback));
    hookPopUp->Add(this);
    
    quitPopUp = new PopUp("Warning: Unsaved Changes", "Unsaved changes will be lost.\nDo you want to exit?", this, menu_selector(LevelEditor::acceptQuitCallback), menu_selector(LevelEditor::declineQuitCallback));
    quitPopUp->Add(this);
    
    this->addChild(currentSprite);
    
    currentSelection = NULL;
    currentTool = NO_TOOL;
    UpdateAxis();
    return true;
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
void LevelEditor::rotateCallback(){
    isRotating = true;
};
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
}
void LevelEditor::saveAcceptCallback(Ref* ref){
    name = savePopUp->GetText();
    Export();
    saveDialog = false;
    savePopUp->Close();
}
void LevelEditor::saveDeclineCallback(Ref* ref){
    saveDialog = false;
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
            if(transform->getBoundingBox().containsPoint(touchStart))
               transformCallback();
            if(rotate->getBoundingBox().containsPoint(touchStart))
                rotateCallback();
            if(resize->getBoundingBox().containsPoint(touchStart))
                resizeCallback();
            //Check if the user selected an entity//
            Entity* target = preview->GetTarget(touchStart);
            if(target != nullptr){
                Select(target);
            } else {
            //End Check//
                if(isRotating || isTransforming || isScaling){
                    
                } else {
                    if(currentTool == HOOK || currentTool == SPAWNER){
                        Entity* tempEnt = (preview->CreateEntity(touchStart, touchStart, currentTool));
                        entities[tempEnt->ID] = tempEnt;
                        preview->AddEntity(tempEnt);
                    } else {
                        if(currentTool == NO_TOOL){
                            if(hasSelected)
                                Deselect();
                        } else {
                            currentSprite->setPosition(touchStart);
                            currentSprite->setContentSize(Size(1,1));
                            currentSprite->setVisible(true);
                        }
                    }
                } // This may be innapropriately placed.
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
    //if(entity->GetType() == HOOK || entity->GetType() == SPAWNER)
        //resize->setEnabled(false);
    //if(entity->GetType() == HOOK)
        //rotate->setEnabled(false);
    hasSelected = true;
    currentSprite->setVisible(true);
    selectedEntity = entity;
    Vec2 startPosition = preview->MapToScreen(selectedEntity->GetPosition());
    Size selectedEntitySize = Size(preview->GetScale() * entity->GetSize().x, preview->GetScale() * selectedEntity->GetSize().y);
    startPosition.x -= selectedEntitySize.width / 2.0;
    startPosition.y -= selectedEntitySize.height / 2.0;
    currentSprite->setPosition(startPosition);
    currentSprite->setContentSize(selectedEntitySize);
    rotate->setVisible(true);
    resize->setVisible(true);
    transform->setVisible(true);
    remove->setVisible(true);
    Rect box = currentSprite->getBoundingBox();
    rotate->setPosition(Vec2(box.getMinX(), box.getMaxY()));
    resize->setPosition(Vec2(box.getMaxX(), box.getMaxY()));
    transform->setPosition(Vec2(box.getMinX(), box.getMinY()));
    remove->setPosition(Vec2(box.getMaxX(), box.getMinY()));
}
void LevelEditor::Deselect(bool keep){
    if(keep){
        preview->AddEntity(selectedEntity);
    }
    hasSelected = false;
    selectedEntity = nullptr;
    transform->setVisible(false);
    rotate->setVisible(false);
    remove->setVisible(false);
    resize->setVisible(false);
    currentSprite->setVisible(false);
    //rotate->setEnabled(true);
    //resize->setEnabled(true);
    //transform->setEnabled(true);
    //remove->setEnabled(true);
    //resize->getEventDispatcher()->setEnabled(false);
    //rotate->getEventDispatcher()->setEnabled(false);
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
        rotate->setPosition(Vec2(box.getMinX(), box.getMaxY()));
        resize->setPosition(Vec2(box.getMaxX(), box.getMaxY()));
        transform->setPosition(Vec2(box.getMinX(), box.getMinY()));
        remove->setPosition(Vec2(box.getMaxX(), box.getMinY()));
    }
}
void LevelEditor::UpdateAxis(){
    Vec2 originPos = preview->MapToScreen(Vec2(0,0));
    verticalAxis->setPosition(originPos.x, verticalAxis->getPosition().y);
    horizontalAxis->setPosition(horizontalAxis->getPosition().x, originPos.y);
    for(int x = 0; x < 100; x++){
        Vec2 tempPos = preview->MapToScreen(Vec2(250 * (-10 + x), 250 * (-10 + x)));
        verticalGuides[x]->setPosition(tempPos.x, verticalGuides[x]->getPosition().y);
        horizontalGuides[x]->setPosition(horizontalGuides[x]->getPosition().x, tempPos.y);
        float tempScale = preview->GetScale() * 1;
        if(tempScale > 1)
            tempScale = 1;
        if(tempScale < 0.5)
            tempScale = 0.5;
        horizontalGuides[x]->setScaleY(tempScale);
        verticalGuides[x]->setScaleY(tempScale);
    }
}
void LevelEditor::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    if(isScaling || isRotating || isTransforming){
        isScaling = false;
        isRotating = false;
        isTransforming = false;
    } else {
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
}
void LevelEditor::onTouchMoved(Touch* touch, Event* event){
    if(isRotating || isScaling || isTransforming){
        Vec2 oldTouch = touchCurrent;
        touchCurrent = touch->getLocation();
        if(isScaling){
            if(currentSprite != NULL){
                currentSprite->setContentSize(Size(currentSprite->getContentSize().width + (touchCurrent.x - oldTouch.x), currentSprite->getContentSize().height - (touchCurrent.y - oldTouch.y)));
                selectedEntity->SetSize(Vec2(currentSprite->getContentSize().width / preview->GetScale(), currentSprite->getContentSize().height / preview->GetScale()));
                this->UpdateSelected();
            }
        }
        if(isTransforming){
            if(currentSprite != NULL){
                currentSprite->setPosition(Vec2(currentSprite->getPosition().x + (touchCurrent.x - oldTouch.x), currentSprite->getPosition().y + (touchCurrent.y - oldTouch.y)));
                selectedEntity->SetPosition(preview->ScreenToMap(Vec2(currentSprite->getBoundingBox().getMidX(), currentSprite->getBoundingBox().getMidY())));
                this->UpdateSelected();
            }
        }
        if(isRotating){
            
        }
    } else {
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
    unsavedChanges = false;
}