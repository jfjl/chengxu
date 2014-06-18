//
//  BasicProps.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__BasicProps__
#define __touchball__BasicProps__

#include "gameConst.h"
#include "ScriptObject.h"
#include "cocos2d.h"

using namespace cocos2d;

class BasicProps : public ScriptObject, public CCSprite
{
private:
    BasicProps();
    ~BasicProps();
public:
    virtual void onUse();
};

#endif /* defined(__touchball__BasicProps__) */
