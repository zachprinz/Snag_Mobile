//
//  Wall.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/29/14.
//
//

#ifndef __Snag__Wall__
#define __Snag__Wall__

#include <iostream>
#include "Entity.h"

class Wall : public Entity{
public:
    Wall(Vec2 position, Vec2 size);
    void update(float dt);
    static int type;
private:

};


#endif /* defined(__Snag__Wall__) */
