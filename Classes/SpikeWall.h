//
//  SpikeWall.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/30/14.
//
//

#ifndef __Snag__SpikeWall__
#define __Snag__SpikeWall__

#include <iostream>
#include "Entity.h"

class SpikeWall : public Entity{
public:
    SpikeWall(Vec2 pos, Vec2 size);
    static int type;
private:
    
};

#endif /* defined(__Snag__SpikeWall__) */
