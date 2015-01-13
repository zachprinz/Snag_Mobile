#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#define LEVELS_CUSTOM 0
#define LEVELS_SOCIAL 1
#define LEVELS_FAVORITED 2
#define LEVELS_RISING 3

#include "cocos2d.h"
#include "User.h"
#include "DynamicLight.h"

class Entity;
class Level;
class PopUp;
class Game : public cocos2d::Layer
{
public:
    static std::vector<std::vector<Level*>> levels;
    static void AddCustomLevel(Level*);
    static void AddRisingLevel(Level*);
    static void AddSocialLevel(Level*);
    static void AddFavoritedLevel(Level*);
    static void SetUpLevels();
    static bool areLevelsSetUp;
    static Game* Instance;
    static cocos2d::Scene* createScene();
    static Scene* myScene;
    virtual bool init();
    void menuCloseCallback(cocos2d::Ref* pSender);
    CREATE_FUNC(Game);
    void setPhyWorld(PhysicsWorld* world2);
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onTouchEnded(cocos2d::Touch* touch, cocos2d::Event* event);
    void onContactPostSolve(PhysicsContact& contact);
    bool onContactBegin(PhysicsContact& contact);
    void UpdateTimer(float);
    void resetButtonCallback(Ref*);
    void homeButtonCallback(Ref*);
    void winLevelSelectCallback(Ref*);
    void winHighscoresSelectCallback(Ref*);
    void winReplaySelectCallback(Ref*);
    void AddJoint(PhysicsJointDistance*);
    void RemoveAllJoints();
    
    virtual void update(float dt);
    void Reset(Level* lvl);
    void Clear();
    void onWin();
    float time;
    Entity* GetClosestHook(Vec2 pos);
    Level* currentLevel;
    User* user;
    std::vector<Layer*> layers;
    ParticleBatchNode* particleBatchNode;
    Vec2 focusPoint;
    void UpdateFocusPoint();
    bool lowShift;
    float MAX_SHIFT;
    float closing;
    void ShiftHigh();
    float startClosingDistance;
    float startClosingMAX_SHIFT;
    bool catching;
    float previousTrail;
    void CreateOcclusionMap(RenderTexture*);
    avalon::graphics::DynamicLight* light;
    Vec2 toOnscreenPosition(Vec2);
    RenderTexture* finalGameTexture;
    Sprite* finalGameSprite;
    RenderTexture* occlusion;
    Sprite* occlusionSprite;
    Vec2 offset;
    std::vector<Vec2> ogPositions;
    bool raycastCallback(PhysicsWorld& world, const PhysicsRayCastInfo& info, void* data);
    DrawNode* _drawnode;
    Camera* camera;
    Layer* uiLayer;
private:
    std::vector<Sprite*> gameSprites;
    std::vector<RenderTexture*> gameTextures;
    MenuItemImage* resetButton;
    MenuItemImage* homeButton;
    bool winPopUpAdded;
    PopUp* winPopUp;
    PhysicsWorld* world;
    Size visibleSize;
    Label* timeLabel;
    void LoadLevel(Level*);
    float scale;
};

#endif // __HELLOWORLD_SCENE_H__
