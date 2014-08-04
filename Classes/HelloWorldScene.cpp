#include "HelloWorldScene.h"
#include "LevelSelect.h"
#include "MainMenu.h"

USING_NS_CC;

Scene* HelloWorld::myScene;

Scene* HelloWorld::createScene()
{
    auto scene = Scene::createWithPhysics();
    //scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto layer = HelloWorld::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    myScene = scene;
    
    scene->addChild(layer);
    
    return scene;
}

void HelloWorld::update(float dt){
    board->update(dt);
}

void HelloWorld::setPhyWorld(PhysicsWorld* worlda){
    world = worlda;
    world->setSpeed(3.0f);
    board->SetPhysicsWorld(world);
    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactPostSolve = CC_CALLBACK_1(HelloWorld::onContactPostSolve, this);
    contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void HelloWorld::onContactPostSolve(PhysicsContact& contact){
    board->onContactPostSolve(contact);
}

bool HelloWorld::onContactBegin(PhysicsContact& contact){
    return board->onContactBegin(contact);
}

bool HelloWorld::init()
{
    if(!Layer::init()){
        return false;
    }
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
    
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener,this);
    
    float tempScale = 1.0;
    currentCenter.set(Director::getInstance()->getWinSize().width / (2 * tempScale), Director::getInstance()->getWinSize().height / (2 * tempScale));
    
    board = new Board(this, world, visibleSize, origin);
    this->addChild(board->timeLabel);
    schedule(schedule_selector(HelloWorld::update));
    this->schedule(schedule_selector(HelloWorld::UpdateTimer),0.1f);
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto resetButton = MainMenu::CreateButton("reset.png", this, menu_selector(HelloWorld::resetButtonCallback), Vec2(0.02, 1.0-0.02), Vec2(1,1));
    menuItems.pushBack(resetButton);
    auto homeButton = MainMenu::CreateButton("home.png", this, menu_selector(HelloWorld::homeButtonCallback), Vec2(1.0-0.02, 1.0-0.02), Vec2(1,1));
    homeButton->setAnchorPoint(Point(1,1));
    menuItems.pushBack(homeButton);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    return true;
}

void HelloWorld::UpdateTimer(float dt){
    Board::Instance->UpdateTimer(dt);
}

void HelloWorld::resetButtonCallback(Ref* ref){
    board->user->Reset();
}

void HelloWorld::homeButtonCallback(Ref* ref){
    if(LevelSelect::myScene == NULL){
        Board::Print("Play button click registered.");
        auto scene = LevelSelect::createScene();
        Director::getInstance()->pushScene(scene);
        LevelSelect::Instance->Refresh();
    }
    else{
        Director::getInstance()->pushScene(LevelSelect::myScene);
        LevelSelect::Instance->Refresh();
    }
}

bool HelloWorld::onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event){
    board->onTouchBegin();
    return true;
}
void HelloWorld::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    board->onTouchEnd();
}

void HelloWorld::menuCloseCallback(Ref* pSender)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WP8) || (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT)
	MessageBox("You pressed the close button. Windows Store Apps do not implement a close button.","Alert");
    return;
#endif

    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
