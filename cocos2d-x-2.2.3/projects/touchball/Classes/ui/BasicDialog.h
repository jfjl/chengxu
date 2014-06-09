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
#include "cocos2d.h"
#include "cocos-ext.h"

using namespace cocos2d;
using namespace extension;

class BasicDialog : public CCLayer
{
public:
    virtual bool init();
    
    virtual void onShow(CCNode* pParent);
    virtual void onHide(bool bRemove);
};

#endif /* defined(__touchball__BasicDialog__) */
