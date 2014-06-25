//
//  PropsDialog.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__PropsDialog__
#define __touchball__PropsDialog__

#include "BasicDialog.h"

class PropsDialog : public BasicDialog
{
public:
    PropsDialog();
    ~PropsDialog();
    
    bool init();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(PropsDialog, create);
    
    void onShow(CCNode* pParent);
private:
    void onClickProps(CCObject * sender, CCControlEvent controlEvent);
    CCArray* m_btnArray;
};
#endif /* defined(__touchball__PropsDialog__) */
