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

const GLchar* shader =
#include "shaders/white.fsh"
const GLchar* pass =
#include "shaders/pass.vsh"
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
    UpdateFocusPoint();
    scale = visibleSize.height / (user->GetPosition().y + (visibleSize.height / 3.5));
    float over = 0;
    if(scale > 1)
        scale = 1;
    if(scale < 0.5){
        scale = 0.5;
        over = (user->GetPosition().y + visibleSize.height/3.5) - 2.0*(visibleSize.height);
    }
    float targetScale = scale;
    user->update(targetScale);
    
    gameTexture->beginWithClear(0,0,0,0);
    for(int x = 0; x < layers.size(); x++){
        layers[x]->setScale(0.5);
        layers[x]->setPosition((visibleSize.width /2) - ((focusPoint.x * 0.5)),0.5 * over);
        layers[x]->setVisible(true);
        layers[x]->visit();
    }
    gameTexture->end();
    gameSprite->setScale(targetScale);
    for(int x = 0;x < layers.size();x++){
        layers[x]->setVisible(false);
    }
};
void Game::UpdateUniforms(){
    _program->use();
    GLint res = _program->getUniformLocationForName("blur");
    _program->setUniformLocationWith1f(res, 3);
    
    glUniform1i(glGetUniformLocation(_program->getProgram(), "u_texture2"), 1);
    
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, gameTexture->getSprite()->getTexture()->getName());
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, gameTexture->getSprite()->getTexture()->getName());

    UpdateShader();
}
void Game::UpdateShader(){
    gameSprite->setVisible(true);
    gameTexture->getSprite()->setGLProgram(_program);
    postShaderGameTexture->beginWithClear(0.0, 0.0, 0.0, 0.0);
    gameSprite->visit();
    postShaderGameTexture->end();
    gameSprite->setVisible(false);
    postShaderGameSprite->setVisible(true);
}
Vec2 GetTween(Vec2 a, Vec2 b, float percent){
    return Vec2((a.x*(1-percent) + b.x*percent), (a.y*(1-percent) + b.y*percent));
}
Vec2 GetDifference(Vec2 a, Vec2 b){
    return Vec2(b.x-a.x,b.y-a.y);
}
void Game::UpdateFocusPoint(){
    //MAX_SHIFT = 30;
    if(lowShift){
        if(MAX_SHIFT > 15)
            MAX_SHIFT = 15;//-= 0.5;
        user->focusPointSprite->setColor(Color3B(0,255,0));
    } else {
        user->focusPointSprite->setColor(Color3B(255,0,0));
    }
    Vec2 targetFocusPoint = Vec2(user->GetSprite()->getBoundingBox().getMidX(), user->GetSprite()->getBoundingBox().getMidY());
    if(!user->isHooked){
        closing = false;
        float velocityPercent = 0.5 * 1.25;
        targetFocusPoint.x += user->GetSprite()->getPhysicsBody()->getVelocity().x * velocityPercent;
        targetFocusPoint.y += user->GetSprite()->getPhysicsBody()->getVelocity().y * velocityPercent;
        float tempDistance = 0;
        for(int x = 0; x < currentLevel->ents.size(); x++){
            if(currentLevel->ents[x]->GetType() == WALL){
                bool runClosing = false;
                bool recordDist = false;
                while(currentLevel->ents[x]->GetSprite()->getBoundingBox().containsPoint(targetFocusPoint) && velocityPercent != 0){
                    velocityPercent -= 0.0125 * 1.25;
                    targetFocusPoint = Vec2(user->GetSprite()->getBoundingBox().getMidX(), user->GetSprite()->getBoundingBox().getMidY());
                    targetFocusPoint.x += user->GetSprite()->getPhysicsBody()->getVelocity().x * velocityPercent;
                    targetFocusPoint.y += user->GetSprite()->getPhysicsBody()->getVelocity().y * velocityPercent;
                    runClosing = true;
                }
                if(runClosing){
                    if(!closing){
                        recordDist = true;
                        closing = true;
                        user->focusPointSprite->setColor(Color3B(0,255,0));
                    }
                    velocityPercent -= 0.0125 * 1.25 * 2;
                    targetFocusPoint = Vec2(user->GetSprite()->getBoundingBox().getMidX(), user->GetSprite()->getBoundingBox().getMidY());
                    targetFocusPoint.x += user->GetSprite()->getPhysicsBody()->getVelocity().x * velocityPercent;
                    targetFocusPoint.y += user->GetSprite()->getPhysicsBody()->getVelocity().y * velocityPercent;
                }
                tempDistance = Vec2(targetFocusPoint.x - user->GetPosition().x, targetFocusPoint.y - user->GetPosition().y).length();
                if(recordDist){
                    startClosingDistance = tempDistance;
                    startClosingMAX_SHIFT = MAX_SHIFT;
                }
            }
        }
        if(closing){
            MAX_SHIFT =(startClosingMAX_SHIFT*(tempDistance / startClosingDistance));
        }
    } else {
        targetFocusPoint = GetTween(targetFocusPoint, user->GetClosestPosition(),0.5f);
        MAX_SHIFT += 0.5;
    }
    Vec2 angleTemp1(targetFocusPoint.x - focusPoint.x, targetFocusPoint.y - focusPoint.y);
    Vec2 angleTemp2(user->GetPosition().x - focusPoint.x, user->GetPosition().y - focusPoint.y);
    if(lowShift && !user->isHooked && angleTemp1.getAngle(angleTemp2) < 0.125){
        lowShift = false;
    }
    float distance = targetFocusPoint.distance(focusPoint);
    if(distance > MAX_SHIFT){
        Vec2 normalizedDifference = GetDifference(focusPoint, targetFocusPoint);
        Vec2 prevTrailVec = normalizedDifference;
        normalizedDifference.normalize();
        focusPoint.x += MAX_SHIFT * normalizedDifference.x;
        focusPoint.y += MAX_SHIFT * normalizedDifference.y;
        if(!catching){
            catching = true;
        }
        if(catching){
            float currentTrail = prevTrailVec.length();
            if(currentTrail > previousTrail){
                MAX_SHIFT += 0.35 * (currentTrail/previousTrail);
            } else {
                MAX_SHIFT -= 0.35 * (currentTrail/previousTrail);
            }
            if(currentTrail > 800){
                catching = false;
                closing = false;
                lowShift = false;
                focusPoint = targetFocusPoint;
                return;
            }
            previousTrail = currentTrail;
        }
    } else {
        focusPoint = targetFocusPoint;
        lowShift = false;
        catching = false;
    }
}
void Game::ShiftHigh(){
    lowShift = false;
}
Scene* Game::myScene;
Scene* Game::createScene() {
    auto scene = Scene::createWithPhysics();    
    auto layer = Game::create();
    layer->setPhyWorld(scene->getPhysicsWorld());
    myScene = scene;
    //myScene->retain();
    scene->addChild(layer);
    return scene;
}
bool Game::onContactBegin(PhysicsContact& contact){
    int type = contact.getShapeB()->getBody()->getTag();
    if(type == SPIKE_WALL){
        //user->Reset();
        user->GetSprite()->setPosition(Vec2(0,-1000));
        return false;
    }
    if(type == GOAL){
        onWin();
        return false;
    }
    if(type == WALL){
        closing = false;
        user->focusPointSprite->setColor(Color3B(255,0,0));
        user->Bounce(contact.getContactData());
        lowShift = true;
    }
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
    winPopUp->Close();
    world->setSpeed(2.0);
}
void Game::setPhyWorld(PhysicsWorld* world2){
    world = world2;
    world->setGravity(Vec2(0,-270));
    world->setSpeed(2.0);
    //world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    for(int x = 0; x < user->joints.size(); x++){
        world->addJoint(user->joints[x]);
    }
    for(int x = 0; x < user->distanceJoints.size(); x++){
        world->addJoint(user->distanceJoints[x]);
    }
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
    
    postShaderGameTexture = RenderTexture::create(visibleSize.width, visibleSize.height);
    postShaderGameTexture->setKeepMatrix(true);
    postShaderGameTexture->retain();
    postShaderGameSprite = Sprite::createWithTexture(gameTexture->getSprite()->getTexture());
    postShaderGameSprite->setPosition(Vec2(visibleSize.width / 2.0, 0));
    postShaderGameSprite->setAnchorPoint(Vec2(0.5,0));
    postShaderGameSprite->setFlippedY(true);
    this->addChild(postShaderGameSprite,1);
    postShaderGameSprite->setVisible(false);
    
    _program = avalon::graphics::loadShader(pass, shader);
    
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
    MAX_SHIFT = 30;
    
    
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
    world->removeJoint(user->joint);
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
        auto transition = TransitionFade::create(MainMenu::transitionTime, LevelSelect::myScene);
        Director::getInstance()->pushScene(transition);
        LevelSelect::Instance->Refresh();
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
        Director::getInstance()->pushScene(transition);
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
    if(!user->isHooked && !winPopUp->visible){
        user->Snag();
        lowShift = true;
        Vec2 touchPosition = touch->getLocation();
        if(resetButton->getBoundingBox().containsPoint(touchPosition) || homeButton->getBoundingBox().containsPoint(touchPosition)){
            return false;
        }
        return true;
    }
}
void Game::onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event){
    user->Release();
    lowShift = true;
}