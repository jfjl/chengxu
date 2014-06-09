//
//  ScoreDialog.h
//  pball
//
//  Created by sora on 13-4-17.
//
//

#ifndef __pball__ScoreDialog__
#define __pball__ScoreDialog__

#include "BasicDialog.h"


class ScoreDialog : public BasicDialog, public CCBMemberVariableAssigner, public CCBSelectorResolver
{
private:
    CCLabelTTF* m_Score;
    CCLabelTTF* m_MaxScore;
    CCLabelTTF* m_TopName;
    
    void doChangeScore(CCObject* data);
public:
    ScoreDialog();
    ~ScoreDialog();
    
    virtual void onShow(CCNode* pParent);
    
    bool init();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ScoreDialog, create);
    
    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
    
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual extension::SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);
  
    void loadTopRecord();
};

#endif /* defined(__pball__ScoreDialog__) */
