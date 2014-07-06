//
//  Goal.h
//  Snag
//
//  Created by Zachary Prinzbach on 7/6/14.
//
//

#ifndef __Snag__Goal__
#define __Snag__Goal__

#include <iostream>
#include "Entity.h"

class Goal : public Entity{
public:
    Goal(Vec2 position, Vec2 size);
    void update(float dt);
    static int type;
private:
    
};

#endif /* defined(__Snag__Goal__) */
