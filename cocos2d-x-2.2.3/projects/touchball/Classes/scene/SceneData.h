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

using namespace cocos2d;

class SceneData : public CCLayer
{
public:    
    virtual bool init();
public:
    CREATE_FUNC(SceneData);
    static SceneData * scene();

    virtual void onUpdate(float dt);

    virtual void onActivate(CCNode* pNode);
    virtual void onDeactivate();
};

#endif /* defined(__touchball__SceneData__) */
