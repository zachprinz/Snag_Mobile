//
//  Spawner.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/30/14.
//
//

#ifndef Snag_Spawner_h
#define Snag_Spawner_h

#include "Entity.h"

class Spawner : public Entity{
public:
    Spawner(Vec2 pos, Vec2 vel);
    Vec2 GetStartVelocity();
    static Spawner* Instance;
private:
    Vec2 velocity;
};


#endif
