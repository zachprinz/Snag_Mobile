//
//  Board.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#ifndef __Snag__Board__
#define __Snag__Board__

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

USING_NS_CC;

class Board{
public:
    Board(Layer* game, PhysicsWorld* world, Size size, Point origin);
    void AddEntity(Entity*);
    static Board* Instance;
    void AddHook(Hook*);
    void AddWall(Wall*);
    void AddGoal(Goal*);
    void AddSpikeWall(SpikeWall*);
    void AddJoint(PhysicsJointDistance*);
    void AddSpawner(Spawner*);
    void RemoveJoint();
    Hook* GetClosestHook(Vec2 pos);
    void onTouchBegin();//Touch* touch, Event* event);
    void onTouchEnd();//Touch* touch, Event* event);
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
    static std::vector<std::string> myLevels;
    static std::vector<std::string> myLevelNames;
    static std::vector<std::string> onlineLevels;
    static std::vector<std::string> onlineLevelNames;
    static std::vector<std::string> localLevels;
    static std::vector<std::string> localLevelNames;
    void Reset(std::string levelPath);
    void Clear();
    void onWin();
    float time;
    void UpdateTimer(float);
private:
    void LoadLevel(std::string name);
    Sprite* background;
    void updateView(float dt);
    void updateCollision(float dt);
    Size boardSize;
    Point origin;
    float scale;
    std::vector<Hook*> hooks;
    std::vector<Wall*> walls;
    std::vector<SpikeWall*> spikewalls;
    std::vector<Spawner*> spawners;
    std::vector<Goal*> goals;
    float ground;
    std::vector<Entity*> ents;
};

#endif /* defined(__Snag__Board__) */
