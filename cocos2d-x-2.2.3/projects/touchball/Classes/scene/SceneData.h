//
//  SceneData.h
//  touchball
//
//  Created by carlor on 14-6-6.
//
//

#ifndef __touchball__SceneData__
#define __touchball__SceneData__

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ScriptObject.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../script/ScriptObject.h"
#endif

using namespace cocos2d;

class SceneData : public Object, public CCLayer
{
public:    
    virtual bool init();
public:
    CREATE_FUNC(SceneData);
    static SceneData * scene();

    virtual bool onUpdate(float dt);

    virtual void onActivate(CCNode* pNode, void* param);
    virtual void onDeactivate();
    
    virtual void start(void* param);
protected:
    float m_nRefreshTime;
    float m_nLastRefreshTime;
};

#endif /* defined(__touchball__SceneData__) */
