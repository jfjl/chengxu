//
//  CongratulationDialog.cpp
//  pball
//
//  Created by sora on 13-4-17.
//
//

#include "CongratulationDialog.h"

CongratulationDialog::CongratulationDialog()
{
    
}

CongratulationDialog::~CongratulationDialog()
{
    
}

bool CongratulationDialog::init()
{
    if (! CCLayer::init()){
        return false;
    }
    CCBReader* reader = new CCBReader(CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary());
    reader->autorelease();
    CCNode* congratulationDialog = reader->readNodeGraphFromFile("CongratulationDialog.ccbi", this);
    this->addChild(congratulationDialog);
    
    return true;
}

void CongratulationDialog::onClickComplate(CCObject *node, CCControlEvent event)
{
    CCLog("click complate");
}

//CCBSelectorResolver
SEL_MenuHandler CongratulationDialog::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCLog("MenuItem Selector");
	
	return NULL;
}

SEL_CCControlHandler CongratulationDialog::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    CCB_SELECTORRESOLVER_CCCONTROL_GLUE( this, "onClickComplate", CongratulationDialog::onClickComplate);

	return NULL;
}

//CCBMemberVariableAssigner
bool CongratulationDialog::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
//    CCB_MEMBERVARIABLEASSIGNER_GLUE( this, "m_TestLabel", CCLabelTTF*, m_TestLabel );
	return false;
}
