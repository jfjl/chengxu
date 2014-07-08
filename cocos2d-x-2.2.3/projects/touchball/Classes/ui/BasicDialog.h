//
//  BasicDialog.h
//  touchball
//
//  Created by carlor on 14-6-7.
//
//

#ifndef __touchball__BasicDialog__
#define __touchball__BasicDialog__

#include "cocos-ext.h"
#include "DialogEvent.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "gameConst.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../utils/gameConst.h"
#endif

using namespace cocos2d;
using namespace extension;

class BasicDialog : public CCNode
{
public:
    void release(void);
    
    virtual bool init();
    
    virtual void onShow(void* param);
    virtual void onHide(void* param);
    virtual bool onUpdate(float dt);
    
    void setName(const char* name);
    
protected:
    virtual void initDialog();

	CC_SYNTHESIZE_READONLY(const char*, m_name, Name);
    CC_SYNTHESIZE_READONLY(ui::Layout*, m_pDialog, Dialog);
    CC_SYNTHESIZE_READONLY(float, m_nRefreshTime, RefreshTime);
    CC_SYNTHESIZE_READONLY(float, m_nLastRefreshTime, LastRefreshTime);
    
};

#endif /* defined(__touchball__BasicDialog__) */
