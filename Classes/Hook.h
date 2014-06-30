//
//  Hook.h
//  Snag
//
//  Created by Zachary Prinzbach on 6/28/14.
//
//

#ifndef __Snag__Hook__
#define __Snag__Hook__

#include <iostream>
#include "cocos2d.h"
#include "Entity.h"

USING_NS_CC;

class Hook : public Entity{
public:
    Hook(Vec2);
    void update(float dt);
private:
    
};

#endif /* defined(__Snag__Hook__) */
