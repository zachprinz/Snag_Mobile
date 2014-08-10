#include "Game.h"
#include "LevelSelect.h"
#include "MainMenu.h"
#include "string.h"
#include "Level.h"
#include <iostream>
#include <iomanip>
#include "Entity.h"

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
    Vec2 userPos = currentLevel->user->GetPhysicalPosition();
    float dist = userPos.y;
    if(dist < visibleSize.height)
        dist = visibleSize.height;
    scale = 1.0 / ((visibleSize.height - visibleSize.height * 0.15) / dist);
    currentLevel->user->update(scale);
    for(int x = 0; x < currentLevel->ents.size(); x++){
        currentLevel->ents[x]->update(userPos, scale);
    }
    currentLevel->user->update(scale);
}
Scene* Game::myScene;

Scene* Game::createScene() {
    auto scene = Scene::createWithPhysics();
    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    auto layer = Game::create();
    myScene = scene;
    scene->addChild(layer);
    return scene;
}

void Game::onContactPostSolve(PhysicsContact& contact){
    int type = contact.getShapeB()->getBody()->getTag();
    if(type == WALL)
        currentLevel->user->Bounce(contact.getContactData());
    if(type == SPIKE_WALL)
        currentLevel->user->Reset();
}
bool Game::onContactBegin(PhysicsContact& contact){
    if(contact.getShapeB()->getBody()->getTag() == -1){
        onWin();
        return false;
    }
    currentLevel->user->SetBackupVelocity();
    return true;
}
void Game::onWin(){

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
    currentLevel->user->Reset();
}
void Game::Clear(){
    currentLevel->Remove(this);
}
void Game::LoadLevel(Level* lvl){
    currentLevel = lvl;
    currentLevel->Add(this);
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
    //this->getScene()->getPhysicsWorld()->setSpeed(3.0);
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
    this->addChild(background);
    
    timeLabel = Label::createWithBMFont("dimbo.fnt", "0.00", TextHAlignment::LEFT);
    timeLabel->setPosition(visibleSize.width / 2.0 - (80 * MainMenu::screenScale.x), visibleSize.height);
    timeLabel->setAnchorPoint(Point(0.0,1.0));
    this->addChild(timeLabel);
    
    auto resetButton = MainMenu::CreateButton("reset.png", this, menu_selector(Game::resetButtonCallback), Vec2(0.02, 1.0-0.02), Vec2(1,1));
    menuItems.pushBack(resetButton);
    
    auto homeButton = MainMenu::CreateButton("home.png", this, menu_selector(Game::homeButtonCallback), Vec2(1.0-0.02, 1.0-0.02), Vec2(1,1));
    homeButton->setAnchorPoint(Point(1,1));
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
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
    this->AddJoint(joint);
};
void Game::RemoveAllJoints(){
    this->RemoveAllJoints();
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
    currentLevel->user->Reset();
}
void Game::homeButtonCallback(Ref* ref){
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
bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    return true;
}
void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    
}