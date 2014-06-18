//
//  CongratulationDialog.h
//  pball
//
//  Created by sora on 13-4-17.
//
//

#ifndef __pball__CongratulationDialog__
#define __pball__CongratulationDialog__

#include "BasicDialog.h";

class CongratulationDialog : public BasicDialog, public CCBSelectorResolver, public CCBMemberVariableAssigner
{
public:
    CongratulationDialog();
    ~CongratulationDialog();
    
    bool init();
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(CongratulationDialog, create);
    
    void onClickComplate(CCObject *node, CCControlEvent event);

    //CCBSelectorResolver
    virtual SEL_MenuHandler onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName);
    virtual SEL_CCControlHandler onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName);

    //CCBMemberVariableAssigner
    virtual bool onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode);
};

#endif /* defined(__pball__CongratulationDialog__) */
