#ifndef Snag_Game_h
#define Snag_Game_h

#include "Level.h"

class Game{
public:
    void update(float dt);
    
    void reset();
    void clear();
    bool onContact(PhysicsContact&);
    
    Entity* getClosestHook(Vec2 pos);
    void loadLevel(Level*);

    void shiftLow();
    
    Vec2 getGravity();
    Vec2 getFocusPoint();
    float getScale();
    float getOver();
    
    void release();
    bool snag();

private:
    void updateGravity();
    void updateFocusPoint();
    void updateScale();

    Vec2 getTargetFocusPoint();

    Level* currentLevel;
    Vec2 previousTargetChange;
    Vec2 focusPoint;
    float scale;
    float over;
    Vec2 gravity;
    User* user;

    float startClosingDistance;
    float startClosingMAX_SHIFT;
    float MAX_SHIFT;
    float closing;
    float previousTrail;
    bool catching;
    
    float MAX_CHANGE;

};

#endif
