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
#define PTM_RATIO 512.0

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
    if(user->isHooked){
        if(user->GetSprite()->getPhysicsBody()->getVelocity().y > 0){
            world->setGravity(Vec2(0,-240));
        }
        else{
            world->setGravity(Vec2(0,-270));
        }
    }
    if(!user->isHooked){
        world->setGravity(Vec2(0,-180));
    }
    float oldScale = scale;
        scale = visibleSize.height / (user->GetSprite()->getBoundingBox().getMaxY() + (visibleSize.height / 5.0));
    if(scale > 1.2)
        scale = 1.2;
    if(scale < 0.5)
        scale = 0.5;
    float newScale;
    float MAX_CHANGE = 0.001;
    float targetScale = scale;
    if(abs(targetScale-oldScale) > MAX_CHANGE){
        if(targetScale > oldScale){
            newScale = oldScale + MAX_CHANGE;
        }
        else{
            newScale = oldScale - MAX_CHANGE;
        }
    } else
        newScale = targetScale;
    //for(int x = 0; x < layers.size(); x++){
    //    layers[x]->setScale(newScale);
     //   layers[x]->setPosition(Vec2(0,0));
    //}
    user->update(newScale);

    gameTexture->setScale(1.0);
    gameTexture->beginWithClear(0,0,0,0);
    for(int x = 0; x < layers.size(); x++){
        layers[x]->setScale(0.5);//targetScale);
        layers[x]->setPosition((visibleSize.width /2) - ((user->GetSprite()->getBoundingBox().getMidX())*0.5),0);
        layers[x]->setVisible(true);
        layers[x]->visit();
    }
    gameTexture->end();
    for(int x = 0; x < layers.size(); x++){
        layers[x]->setVisible(false);
    }
    gameSprite->setScale(targetScale);
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
bool Game::onContactBegin(PhysicsContact& contact){
    int type = contact.getShapeB()->getBody()->getTag();
    if(type == SPIKE_WALL)
        user->Reset();
    if(type == GOAL){
        onWin();
        return false;
    }
    if(type == WALL)
        user->Bounce(contact.getContactData());
    return true;
}
void Game::onWin(){
    winPopUp->Show();
    world->setSpeed(0.2);
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
    world->setGravity(Vec2(0,-270));
    world->setSpeed(2.0);
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
    
    for(int x = 0; x < 6; x++){
        Layer* tempLayer = Layer::create();
        tempLayer->setAnchorPoint(Vec2(0.0,0));
        tempLayer->setPosition(0,0);
        layers.push_back(tempLayer);
        this->addChild(tempLayer);
    }
    user = new User();
    user->Add(this);
    
    gameTexture = RenderTexture::createWithPixels(visibleSize.width * 2.0, visibleSize.height*2.0,Texture2D::PixelFormat::RGBA8888, 0);
    gameTexture->setPosition(0,0);
    gameTexture->setKeepMatrix(true);
    gameTexture->retain();
    gameTexture->setVisible(false);
    this->addChild(gameTexture,1);
    
    gameSprite = Sprite::createWithTexture(gameTexture->getSprite()->getTexture());
    gameSprite->setPosition(visibleSize.width / 2.0,0);
    gameSprite->setAnchorPoint(Vec2(0.5,0));
    //gameSprite->setAnchorPoint(Vec2(0.5,0.5));
    gameSprite->retain();
    gameSprite->setFlippedY(true);
   // gameSprite->setRotation(180);
    this->addChild(gameSprite,1);
    
    timeLabel = MainMenu::CreateLabel("0:00", 2);
    timeLabel->setPosition(visibleSize.width / 2.0 - (80 * MainMenu::screenScale.x), visibleSize.height);
    timeLabel->setColor(Color3B::BLACK);
    timeLabel->setAnchorPoint(Point(0.0,1.0));
    timeLabel->setGlobalZOrder(3);
    this->addChild(timeLabel,1);
    
    resetButton = MainMenu::CreateButton("game", "Refresh.png", this, menu_selector(Game::resetButtonCallback));
    resetButton->setPosition(.017*visibleSize.width, visibleSize.height - 0.025*visibleSize.height);
    resetButton->setAnchorPoint(Vec2(0.0,1.0));
    menuItems.pushBack(resetButton);
    
    homeButton = MainMenu::CreateButton("game", "Home.png", this, menu_selector(Game::homeButtonCallback));
    homeButton->setPosition(visibleSize.width - .017*visibleSize.width, visibleSize.height - 0.025*visibleSize.height);
    homeButton->setAnchorPoint(Vec2(1.0,1.0));

    homeButton->setAnchorPoint(Point(1,1));
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    scale = 1.0;
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
    world->setGravity(Vec2(0,-270));
};
void Game::RemoveAllJoints(){
    world->removeAllJoints();
    world->setGravity(Vec2(0,-270));
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
    world->setSpeed(2.0);
    resetButtonCallback(nullptr);
};
bool Game::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    user->Snag();
    Vec2 touchPosition = touch->getLocation();
    if(resetButton->getBoundingBox().containsPoint(touchPosition) || homeButton->getBoundingBox().containsPoint(touchPosition)){
        return false;
    }
    return true;
}
void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    user->Release();
}