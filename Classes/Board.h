//
//  Board.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#ifndef __Snag__Board__
#define __Snag__Board__
#define LEVELS_CUSTOM 0
#define LEVELS_SOCIAL 1
#define LEVELS_FAVORITED 2
#define LEVELS_RISING 3

#include <iostream>
#include "cocos2d.h"
#include <vector>
#include "User.h"
#include "Entity.h"
#include "Hook.h"
#include "Wall.h"
#include "SpikeWall.h"
#include "Spawner.h"
#include "Goal.h"
#include "Level.h"

USING_NS_CC;

class Board{
public:
    Board(Layer* game, PhysicsWorld* world, Size size, Point origin);
    static Board* Instance;
    void AddJoint(PhysicsJointDistance*);
    void AddSpawner(Spawner*);
    void RemoveJoint();
    Hook* GetClosestHook(Vec2 pos);
    void onTouchBegin();
    void onTouchEnd();
    void update(float dt);
    Point center;
    Vec2 gravity;
    Layer* game;
    Size visibleSize;
    static void PrintVec2(std::string name, Vec2);
    User* user;
    PhysicsWorld* world;
    void SetPhysicsWorld(PhysicsWorld* world);
    bool onContactBegin(PhysicsContact& contact);
    void onContactPostSolve(PhysicsContact& contact);
    EventDispatcher* eventDispatcher;
    Label* timeLabel;
    static void Print(std::string);
    static char* levelPath;
    static bool customLevel;
    static std::vector<std::vector<Level*>> levels;
    static void AddCustomLevel(Level*);
    static void AddRisingLevel(Level*);
    static void AddSocialLevel(Level*);
    static void AddFavoritedLevel(Level*);
    static void SetUpLevels();
    static bool areLevelsSetUp;
    void Reset(Level* lvl);
    void Clear();
    void onWin();
    float time;
    void UpdateTimer(float);
    void emptyCallback(Ref*);
private:
    void LoadLevel(Level*);
    Level* currentLevel;
    Sprite* background;
    void updateView(float dt);
    void updateCollision(float dt);
    Size boardSize;
    Point origin;
    float scale;
    float ground;
};

#endif /* defined(__Snag__Board__) */
