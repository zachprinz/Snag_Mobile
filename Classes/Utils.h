#ifndef Snag_Utils_h
#define Snag_Utils_h

#include "cocos2d.h"

float getAngle(Vec2 a, Vec2 b){
    float o = a.x - b.x;
    float adj = a.y - b.y;
    if(a.x == b.x){
        if(a.y > b.y)
            return 90;
        else
            return 270;
    }
    if(a.y == b.y){
        if(a.x > b.x)
            return 0;
        else
            return 180;
    }
    float angle = atan(o/adj) * 57.29;
    if(adj < 0){ //Left two
        if(adj > 0) // bottom left
            angle = (90-angle) + 180;
        if(adj < 0) // top left
            angle = 90 + angle;
    }
    else{ // Right two
        if(adj > 0) // bottom right
            angle += 270;
        if(adj < 0) // top right
            angle = (90 - angle);
    }
    return angle;
}

Vec2 GetTween(Vec2 a, Vec2 b, float percent){
    return Vec2((a.x*(1-percent) + b.x*percent), (a.y*(1-percent) + b.y*percent));
}

Vec2 GetDifference(Vec2 a, Vec2 b){
    return Vec2(b.x-a.x,b.y-a.y);
}

#endif
