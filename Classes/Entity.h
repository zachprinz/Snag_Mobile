#ifndef __Snag__Entity__
#define __Snag__Entity__

#define MAX_WIDTH 2048
#define MAX_HEIGHT 1538
#define WALL 0
#define SPIKE_WALL 1
#define HOOK 2
#define SPAWNER 3
#define GOAL 4
#define USER 5
#define LINE 1000

#include <iostream>
#include "cocos2d.h"

USING_NS_CC;

int ID_MASK   = ~7;
int TYPE_MASK = 7;
int TAG_TO_ID  (int tag) { return  (tag & ID_MASK)>>3; }
int TAG_TO_TYPE(int tag) { return  (tag & TYPE_MASK); }

class Entity{
    protected:
        Entity(Vec2 pos, Vec2 size, Vec2 vel, std::string texture, bool collision);
        virtual void setUpPhysicsSprite(bool collision);
        Sprite*      sprite;
        PhysicsBody* body;
    
    public:
        static Entity* createWall(Vec2, Vec2);
        static Entity* createSpikeWall(Vec2, Vec2);
        static Entity* createGoal(Vec2, Vec2);
        static Entity* createHook(Vec2);
        static Entity* createSpawner(Vec2, Vec2);
        static int count;
    
        virtual void add();
        virtual void remove();
        
        Vec2  getSize();
        void  setSize(Vec2);
        Point getPosition();
        void  setPosition(Vec2);
        Vec2  getLaunchVelocity();
        void  setLaunchVelocity(Vec2);
        int   getID();
        int   getTag();
        void  setTag(int);
        Rect  getBoundingBox();
        PhysicsBody* getPhysicsBody();
    
        ParticleSystemQuad* getEmitter();
        void                setEmitter(ParticleSystemQuad*);
        void             createEmitter();
    
    private:
        int ID;
        int tag;
        Vec2 size;
        Vec2 launchVelocity;
        ParticleSystemQuad* emitter;
};

#endif /* defined(__Snag__Entity__) */
