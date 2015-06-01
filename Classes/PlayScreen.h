#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "Utils.h"
#include "Game.h"

class PopUp;
class PlayScreen : public cocos2d::Layer
{
public:
    CREATE_FUNC(PlayScreen);

    static PlayScreen* Instance;
    static cocos2d::Scene* createScene();
    static Scene* myScene;
    
    virtual bool init();
    virtual void update(float dt);
    
    void setPhyWorld(PhysicsWorld*);
    
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    
    void onContactPostSolve(PhysicsContact&);
    bool onContactBegin(PhysicsContact&);
    
    void         resetButtonCallback(Ref*);
    void          homeButtonCallback(Ref*);
    void      winLevelSelectCallback(Ref*);
    void winHighscoresSelectCallback(Ref*);
    void     winReplaySelectCallback(Ref*);
    void           menuCloseCallback(Ref*);

    void    addSprite(Sprite*);
    void removeSprite(Sprite*);
    void    addEmitter(ParticleSystemQuad*);
    void removeEmitter(ParticleSystemQuad*);
    void    addJoint(PhysicsJointDistance*);
    void removeJoint(PhysicsJointDistance*);
    void removeAllJoints();
    
    void initWinPopUp();
    void onWin();
    Vec2 getVisibleSize();
    Game* getGame();
    void resetTime();
    
    void setGravity(Vec2);
    void setSpeed(float);
    
    void raycast(PhysicsRayCastCallbackFunc, const Point, const Point, void*);

private:
    void updateLayerPositions();
    void UpdateTimer(float);
    ParticleBatchNode* particleBatchNode;
    std::vector<Layer*> layers;
    RenderTexture* gameTexture;
    MenuItemImage* resetButton;
    MenuItemImage* homeButton;
    DrawNode* _drawnode;
    PhysicsWorld* world;
    Sprite* gameSprite;
    bool winPopUpAdded;
    Vec2 visibleSize;
    Label* timeLabel;
    PopUp* winPopUp;
    float time;
    Game* game;
    float scale;
};

#endif // __HELLOWORLD_SCENE_H__
