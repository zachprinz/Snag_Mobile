#include "PlayScreen.h"
#include "LevelSelect.h"
#include "MainMenu.h"
#include "string.h"
#include <iostream>
#include <iomanip>
#include "PopUp.h"
#include "Highscores.h"

#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5
#define PTM_RATIO 256.0

#define WORLD_SPEED_LOW 0.2
#define WORLD_SPEED_HIGH 2.0

USING_NS_CC;

Scene* PlayScreen::myScene;
PlayScreen* PlayScreen::Instance;

Scene* PlayScreen::createScene() {
    auto scene = Scene::createWithPhysics();
    auto layer = PlayScreen::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    myScene = scene;
    myScene->retain();
    scene->addChild(layer);
    return scene;
}

bool PlayScreen::init(){
    if(!Layer::init()){
        return false;
    }
    visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(PlayScreen::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(PlayScreen::onTouchEnded, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    schedule(schedule_selector(PlayScreen::update));
    this->schedule(schedule_selector(PlayScreen::UpdateTimer),0.1f);
    
    Instance = this;
    
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(PlayScreen::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
    
    currentLevel = NULL;
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto background = Sprite::create("background.png");
    background->setPosition(0,0);
    background->setAnchorPoint(Vec2(0,0));
    background->setScale(visibleSize.width / background->getBoundingBox().size.width, visibleSize.height / background->getBoundingBox().size.height);
    background->setGlobalZOrder(-2);
    this->addChild(background);
    
    for(int x = 0; x < 7; x++){
        Layer* tempLayer = Layer::create();
        tempLayer->setAnchorPoint(Vec2(0.0,0));
        tempLayer->setPosition(0,0);
        layers.push_back(tempLayer);
        this->addChild(tempLayer);
    }
    particleBatchNode = ParticleBatchNode::createWithTexture(Director::getInstance()->getTextureCache()->addImage("Images/particle.png"));
    layers[6]->addChild(particleBatchNode);
    user = new User();
    user->Add(this);
    _drawnode = DrawNode::create();
    //layers[5]->addChild(_drawnode,3);
    //this->addChild(_drawnode,3);
    
    timeLabel = MainMenu::CreateLabel("0:00", 2);
    timeLabel->setPosition(visibleSize.width / 2.0 - (80 * MainMenu::screenScale.x), visibleSize.height);
    timeLabel->setColor(Color3B::BLACK);
    timeLabel->setAnchorPoint(Point(0.0,1.0));
    timeLabel->setGlobalZOrder(3);
    this->addChild(timeLabel,1);
    
    gameTexture = RenderTexture::create(visibleSize.width * 2, visibleSize.height * 2);
    gameTexture->setKeepMatrix(true);
    gameTexture->retain();
    gameSprite = Sprite::createWithTexture(gameTexture->getSprite()->getTexture());
    gameSprite->setPosition(Vec2(visibleSize.width / 2.0, 0));
    gameSprite->setAnchorPoint(Vec2(0.5,0));
    gameSprite->setFlippedY(true);
    this->addChild(gameSprite,1);
    
    resetButton = MainMenu::CreateButton("game", "Refresh.png", this, menu_selector(PlayScreen::resetButtonCallback));
    resetButton->setPosition(.017*visibleSize.width, visibleSize.height - 0.025*visibleSize.height);
    resetButton->setAnchorPoint(Vec2(0.0,1.0));
    menuItems.pushBack(resetButton);
    
    homeButton = MainMenu::CreateButton("game", "Home.png", this, menu_selector(PlayScreen::homeButtonCallback));
    homeButton->setPosition(visibleSize.width - .017*visibleSize.width, visibleSize.height - 0.025*visibleSize.height);
    homeButton->setAnchorPoint(Vec2(1.0,1.0));
    
    homeButton->setAnchorPoint(Point(1,1));
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    scale = 1.0;
    winPopUp = new PopUp("You Win!", "What Next?", this, menu_selector(PlayScreen::winLevelSelectCallback), menu_selector(PlayScreen::winHighscoresSelectCallback), menu_selector(PlayScreen::winReplaySelectCallback), 8);
    winPopUpAdded = false;
    MAX_SHIFT = 30/ ((visibleSize.width * visibleSize.height)/(1704.f*960.f));
    previousTargetChange = Vec2(0,0);
    
    return true;
}

void PlayScreen::update(float dt){
    game->update(dt);
    physicsWorld->setGravity(game->getGravity());
    updateLayerPositions();
};

void PlayScreen::updateLayerPositions(){
    Vec2 focusPoint = game->getFocusPoint();
    float scale = game->getScale();
    float over = game->getOver();

    gameTexture->beginWithClear(0,0,0,0);
    for(int x = 0; x < layers.size(); x++){
        layers[x]->setScale(1);
        layers[x]->setPosition(Vec2(1 * ((visibleSize.width /2) - ((focusPoint.x * 1))),-1 * over));
        layers[x]->setVisible(true);
        layers[x]->visit();
    }
    gameTexture->end();
    
    gameSprite->setScale(scale);
    
    //TODO: Try to just setVisible(false) after you visit().
    for(int x = 0;x < layers.size();x++){
        layers[x]->setVisible(false);
    }
}

void PlayScreen::initWinPopUp(){
    if(winPopUpAdded == false){
        winPopUp->Add(this);
        winPopUpAdded = true;
    }
    winPopUp->Close();
    PlayScreen::initWinPopUp();
    world->setSpeed(WORLD_SPEED_HIGH);
}

void PlayScreen::onWin(){
    PlayScreen::Instance->onWin();
    winPopUp->Show();
    world->setSpeed(WORLD_SPEED_LOW);
}

void PlayScreen::resetTime(){
    time = 0.0;
}

bool PlayScreen::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    if(!winPopUp->visible){
        Vec2 touchPosition = touch->getLocation();
        if(resetButton->getBoundingBox().containsPoint(touchPosition) || homeButton->getBoundingBox().containsPoint(touchPosition))
            return false;
        return game->snag();
    }
}
void PlayScreen::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    game->release();
}

bool PlayScreen::onContactBegin(PhysicsContact& contact){
    return game->onContact(contact);
}

void PlayScreen::setGravity(Vec2 gravity){
    physicsWorld->setGravity(gravity);
}
void PlayScreen::setSpeed(float speed){
    physicsWorld->setSpeed(speed);
}
Vec2 PlayScreen::getVisibleSize(){
    return visibleSize;
}

void PlayScreen::raycast(PhysicsRayCastCallbackFunc func, const Point p1, const Point p2, void* data){
    physicsWorld->rayCast(func, p1, p2, data);
}

void PlayScreen::setPhyWorld(PhysicsWorld* world2){
    world = world2;
    world->setGravity(Vec2(0,-270));
    world->setSpeed(WORLD_SPEED_HIGH);
    //world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    for(int x = 0; x < user->joints.size(); x++)
        world->addJoint(user->joints[x]);
    for(int x = 0; x < user->distanceJoints.size(); x++)
        world->addJoint(user->distanceJoints[x]);
}

void PlayScreen::addSprite(Sprite* sprite){
    if(sprite->getTag() != LINE){
        layers[sprite->getTag()]->addChild(sprite,1);
        return;
    }
}
void PlayScreen::removeSprite(Sprite* sprite){
    layers[sprite->getTag()]->removeChild(sprite,1);
}
void PlayScreen::addEmitter(ParticleSystemQuad* emitter){
    particleBatchNode->addChild(emitter);
}
void PlayScreen::removeEmitter(ParticleSystemQuad* emitter){
    particleBatchNode->removeChild(emitter, false);
}
void PlayScreen::AddJoint(PhysicsJointDistance* joint){
    world->addJoint(joint); //world->setGravity(Vec2(0,-270));
};
void PlayScreen::RemoveAllJoints(){
    world->removeJoint(user->joint); //world->setGravity(Vec2(0,-270));
};

void PlayScreen::UpdateTimer(float dt){
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

void PlayScreen::resetButtonCallback(Ref* ref){
    user->Reset();
}
void PlayScreen::homeButtonCallback(Ref* ref){
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(transition);
        LevelSelect::Instance->Refresh();
}
void PlayScreen::winLevelSelectCallback(Ref*){
    winPopUp->Close();
    homeButtonCallback(nullptr);
}
void PlayScreen::winHighscoresSelectCallback(Ref*){
    if(Highscores::myScene == NULL){
        auto scene = Highscores::createScene();
        Highscores::Instance->SetLevel(currentLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, scene);
        Director::getInstance()->pushScene(transition);
    }
    else{
        Highscores::Instance->SetLevel(currentLevel);
        auto transition = TransitionFade::create(MainMenu::transitionTime, Highscores::myScene);
        Director::getInstance()->pushScene(transition);
    }
};
void PlayScreen::winReplaySelectCallback(Ref*){
    winPopUp->Close();
    world->setSpeed(WORLD_SPEED_HIGH);
    resetButtonCallback(nullptr);
};

void PlayScreen::menuCloseCallback(Ref* pSender) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
    MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif
    Director::getInstance()->end();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}