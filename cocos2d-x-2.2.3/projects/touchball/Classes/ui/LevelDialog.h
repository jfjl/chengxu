//
//  LevelDialog.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__LevelDialog__
#define __touchball__LevelDialog__

#include "BasicDialog.h"

class LevelDialog : public BasicDialog
{
public:
    LevelDialog();
    ~LevelDialog();
    
    bool init();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LevelDialog, create);
    
    void onShow(CCNode* pParent);
private:
    virtual void editBoxReturn(CCEditBox* editBox);
    void onClickConfirm(CCObject * sender, CCControlEvent controlEvent);
private:
    CCEditBox*          m_edtLevel;
    CCControlButton*    m_btnConfirm;
};

#endif /* defined(__touchball__LevelDialog__) */
