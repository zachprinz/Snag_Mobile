#include "HelloWorldScene.h"
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
    board->onContactBegin(contact);
    return true;
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
    
    schedule(schedule_selector(HelloWorld::update));
    
    cocos2d::Vector<MenuItem*> menuItems;
    
    auto resetButton = MenuItemImage::create("reset.png", "reset.png", this, menu_selector(HelloWorld::resetButtonCallback));
    resetButton->setPosition(Point(15,visibleSize.height - 15));//57 + (visibleSize.width - 100) / 4.0,- 100));
    resetButton->setAnchorPoint(Point(0.0,1.0));
    menuItems.pushBack(resetButton);
    auto homeButton = MenuItemImage::create("home.png", "home.png", this, menu_selector(HelloWorld::homeButtonCallback));
    homeButton->setPosition(Point(visibleSize.width  - 15, visibleSize.height -15));//52 + ((visibleSize.width - 100) / 4.0) * 3,- 100));
    homeButton->setAnchorPoint(Point(1.0,1.0));
    menuItems.pushBack(homeButton);
    
    resetButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    homeButton->setScale(MainMenu::screenScale.x, MainMenu::screenScale.y);
    
    Menu* menu = Menu::createWithArray(menuItems);
    menu->setAnchorPoint(Point(0.0,0.0));
    menu->setPosition(0,0);
    this->addChild(menu, 1);
    
    return true;
}

void HelloWorld::resetButtonCallback(Ref* ref){
    board->user->Reset();
}

void HelloWorld::homeButtonCallback(Ref* ref){
    Director::getInstance()->pushScene(MainMenu::myScene);
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
