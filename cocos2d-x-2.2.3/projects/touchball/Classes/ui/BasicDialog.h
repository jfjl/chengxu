//
//  BasicDialog.h
//  touchball
//
//  Created by carlor on 14-6-7.
//
//

#ifndef __touchball__BasicDialog__
#define __touchball__BasicDialog__

#include "gameConst.h"
#include "cocos-ext.h"
#include "DialogEvent.h"

using namespace cocos2d;
using namespace extension;

class BasicDialog : public CCLayer
{
public:
    void release(void);
    
    virtual bool init();
    
    virtual void onShow(CCNode* pParent);
    virtual void onHide(bool bRemove);
    
    void setName(const char* name);
protected:
	CC_SYNTHESIZE_READONLY(const char*, m_name, Name);
};

#endif /* defined(__touchball__BasicDialog__) */
