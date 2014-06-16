//
//  ScoreXDialog.h
//  touchball
//
//  Created by carlor on 14-6-16.
//
//

#ifndef __touchball__ScoreXDialog__
#define __touchball__ScoreXDialog__

#include "BasicDialog.h"

class ScoreXDialog : public BasicDialog
{
public:
    ScoreXDialog();
    ~ScoreXDialog();

    bool init();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ScoreXDialog, create);
    
    void onShow(CCNode* pParent);
private:
    void onBallMove(CCObject* pObj);
private:
    CCLabelTTF* m_lblStep;
};

#endif /* defined(__touchball__ScoreXDialog__) */
