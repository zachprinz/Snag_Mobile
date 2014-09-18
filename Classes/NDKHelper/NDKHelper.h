//
//  NDKHelper.h
//  EasyNDK-for-cocos2dx
//
//  Created by Amir Ali Jiwani on 23/02/2013.
//  Updated by Alfonso Cejudo March, 2014.
//
//

#ifndef __EasyNDK_for_cocos2dx__NDKHelper__
#define __EasyNDK_for_cocos2dx__NDKHelper__

#include "cocos2d.h"
#if(CC_PLATFORM_ANDROID == CC_TARGET_PLATFORM)
#include "jansson/jansson.h"
#elif(CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
#include "jansson.h"
#endif
#include "NDKCallbackNode.h"

class NDKHelper
{
public:
    static void addSelector(const char *groupName, const char *name, FuncNV selector, cocos2d::Node *target);
    static void printSelectorList();
    static void removeSelectorsInGroup(const char *groupName);
    
    static cocos2d::Value getValueFromJson(json_t *obj);
    static json_t *getJsonFromValue(cocos2d::Value value);
    
    static void handleMessage(json_t *methodName, json_t *methodParams);
    
private:
    static std::vector<NDKCallbackNode> selectorList;
};

extern "C"
{
    void sendMessageWithParams(std::string methodName, cocos2d::Value methodParams);
}

#endif /* defined(__EasyNDK_for_cocos2dx__NDKHelper__) */
