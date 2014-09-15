#include "Game.h"
#include "LevelSelect.h"
#include "MainMenu.h"
#include "string.h"
#include "Level.h"
#include <iostream>
#include <iomanip>
#include "Entity.h"
#include "PopUp.h"
#include "Highscores.h"

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5

USING_NS_CC;

Game* Game::Instance;
bool Game::areLevelsSetUp;
std::vector<std::vector<Level*>> Game::levels;
void Game::AddCustomLevel(Level* level){
    SetUpLevels();
    levels[LEVELS_CUSTOM].push_back(level);
};
void Game::AddRisingLevel(Level* level){
    SetUpLevels();
    levels[LEVELS_RISING].push_back(level);
};
void Game::AddSocialLevel(Level* level){
    SetUpLevels();
    levels[LEVELS_SOCIAL].push_back(level);
};
void Game::AddFavoritedLevel(Level* level){
    SetUpLevels();
    levels[LEVELS_FAVORITED].push_back(level);
};
void Game::SetUpLevels(){
    if(!areLevelsSetUp){
        for(int x = 0; x < 4; x++){
            std::vector<Level*> levelVector;
            levels.push_back(levelVector);
        }
        areLevelsSetUp = true;
    }
};
void Game::update(float dt){
    Vec2 userPos = user->GetPhysicalPosition();
    float dist = userPos.y;
    if(dist < visibleSize.height)
        dist = visibleSize.height;
    scale = 1.0 / ((visibleSize.height - (visibleSize.height * 0.15)) / dist);
    user->update(scale);
    userPos = user->GetPhysicalPosition();
    for(int x = 0; x < currentLevel->ents.size(); x++){
        currentLevel->ents[x]->update(userPos, scale);
    }
    user->update(scale);
}
Scene* Game::myScene;

Scene* Game::createScene() {
    auto scene = Scene::createWithPhysics();    
    auto layer = Game::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    myScene = scene;
    myScene->retain();
    scene->addChild(layer);
    return scene;
}

void Game::onContactPostSolve(PhysicsContact& contact){
    int type = contact.getShapeB()->getBody()->getTag();
    if(type == WALL)
        user->Bounce(contact.getContactData());
    if(type == SPIKE_WALL)
        user->Reset();
    if(type == GOAL)
        onWin();
}
bool Game::onContactBegin(PhysicsContact& contact){
    if(contact.getShapeB()->getBody()->getTag() == GOAL){
        onWin();
        return false;
    }
    user->SetBackupVelocity();
    return true;
}
void Game::onWin(){
    world->setSpeed(0.2);
    winPopUp->Show();
}
Entity* Game::GetClosestHook(Point pos){
    float smallestDistance = 9999;
    Entity* closestHook;
    for(int x = 0; x < currentLevel->ents.size(); x++){
        if(currentLevel->ents[x]->GetType() == HOOK){
            float distance = currentLevel->ents[x]->GetPosition().getDistance(pos);
            if(distance <= smallestDistance){
                smallestDistance = distance;
                closestHook = (Entity*)currentLevel->ents[x];
            }
        }
    }
    return closestHook;
}
void Game::Reset(Level* lvl){
    if(currentLevel != NULL)
        Clear();
    LoadLevel(lvl);
    user->Reset();
}
void Game::Clear(){
    currentLevel->Remove(this);
}
void Game::LoadLevel(Level* lvl){
    currentLevel = lvl;
    currentLevel->Add(this);
    if(winPopUpAdded == false){
        winPopUp->Add(this);
        winPopUpAdded = true;
    }
}
void Game::setPhyWorld(PhysicsWorld* world2){
    world = world2;
    world->setSpeed(3.0);
    world->setGravity(Vec2(0,-300));
}

bool Game::init(){
    if(!Layer::init()){
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(Game::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(Game::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    schedule(schedule_selector(Game::update));
    this->schedule(schedule_selector(Game::UpdateTimer),0.1f);

    Instance = this;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactPostSolve = CC_CALLBACK_1(Game::onContactPostSolve, this);
    contactListener->onContactBegin = CC_CALLBACK_1(Game::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    currentLevel = NULL;
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto background = Sprite::create("background.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Vec2(0,0));
    background->setScale(visibleSize.width / background->getBoundingBox().size.width, visibleSize.height / background->getBoundingBox().size.height);
    background->setGlobalZOrder(-2);
    this->addChild(background);
    
    
    user = new User();
    user->Add(this);
    
    timeLabel = Label::createWithBMFont("dimbo.fnt", "0.00", TextHAlignment::LEFT);
    timeLabel->setPosition(visibleSize.width / 2.0 - (80 * MainMenu::screenScale.x), visibleSize.height);
    timeLabel->setColor(Color3B::BLACK);
    timeLabel->setAnchorPoint(Point(0.0,1.0));
    timeLabel->setGlobalZOrder(3);
    this->addChild(timeLabel,1);
    
    auto resetButton = MainMenu::CreateButton("reset.png", this, menu_selector(Game::resetButtonCallback), Vec2(0.02, 1.0-0.02), Vec2(1,1));
    menuItems.pushBack(resetButton);
    
    auto homeButton = MainMenu::CreateButton("home.png", this, menu_selector(Game::homeButtonCallback), Vec2(1.0-0.02, 1.0-0.02), Vec2(1,1));
    homeButton->setAnchorPoint(Point(1,1));
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    winPopUp = new PopUp("You Win!", "What Next?", this, menu_selector(Game::winLevelSelectCallback), menu_selector(Game::winHighscoresSelectCallback), menu_selector(Game::winReplaySelectCallback), 8);
    winPopUpAdded = false;
    return true;
}
void Game::menuCloseCallback(Ref* pSender) {
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
        MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
        return;
    #endif
        Director::getInstance()->end();
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}
void Game::AddJoint(PhysicsJointDistance* joint){
    world->addJoint(joint);
};
void Game::RemoveAllJoints(){
    world->removeAllJoints();
};
void Game::UpdateTimer(float dt){
    time += 0.1;
    std::ostringstream out;
    int prec = 2;
    if(((int)time) % 10 >= 1)
        prec = 3;
    if(((int)time) % 100 >= 1)
        prec = 4;
    out << std::setprecision(prec) << time;
    if(fmod(std::atof(out.str().c_str()),1.0) == 0.0)
        out << ".0";
    timeLabel->setString(out.str());
}
void Game::resetButtonCallback(Ref* ref){
    user->Reset();
}
void Game::homeButtonCallback(Ref* ref){
    if(LevelSelect::myScene == NULL){
        auto scene = LevelSelect::createScene();
        Director::getInstance()->pushScene(scene);
        LevelSelect::Instance->Refresh();
    }
    else{
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(transition);
        LevelSelect::Instance->Refresh();
    }
}
void Game::winLevelSelectCallback(Ref*){
    winPopUp->Close();
    world->setSpeed(3.0);
    homeButtonCallback(nullptr);
}
void Game::winHighscoresSelectCallback(Ref*){
    if(Highscores::myScene == NULL){
        auto scene = Highscores::createScene();
        Highscores::Instance->SetLevel(currentLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, scene);
        Director::getInstance()->replaceScene(transition);
    }
    else{
        Highscores::Instance->SetLevel(currentLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, Highscores::myScene);
        Director::getInstance()->pushScene(transition);
    }
};
void Game::winReplaySelectCallback(Ref*){
    winPopUp->Close();
    world->setSpeed(3.0);
    resetButtonCallback(nullptr);
};
bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    user->Snag();
    return true;
}
void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    user->Release();
}