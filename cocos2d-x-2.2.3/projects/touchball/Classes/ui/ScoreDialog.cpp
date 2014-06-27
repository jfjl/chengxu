//
//  ScoreDialog.cpp
//  pball
//
//  Created by sora on 13-4-17.
//
//

#include "ScoreDialog.h"
#include "DialogEvent.h"
#include <stdlib.h>

ScoreDialog::ScoreDialog()
{
    CCLOG("CREATE SCOREDIALOG");
    
}

ScoreDialog::~ScoreDialog()
{
    
}

void ScoreDialog::onShow(CCNode* pParent)
{
    BasicDialog::onShow(pParent);
    this->setPosition(ccp(360, 0));
}


bool ScoreDialog::init()
{
    if (! BasicDialog::init()){
        return false;
    }
    CCNodeLoaderLibrary * ccNodeLoaderLibrary = CCNodeLoaderLibrary::newDefaultCCNodeLoaderLibrary();
    
    /* Create an autorelease CCBReader. */
    cocos2d::extension::CCBReader * ccbReader = new cocos2d::extension::CCBReader(ccNodeLoaderLibrary);
    
    /* Read a ccbi file. */
    CCNode * node = ccbReader->readNodeGraphFromFile("ScoreDialog.ccbi", this);
    
    ccbReader->release();
    
    if (! node) {
        CCLog("load scoreDialog.ccbi failed");
        return false;
    }
    if(node != NULL) {
        this->addChild(node);
    }

    return true;
}

//CCBMemberVariableAssigner
bool ScoreDialog::onAssignCCBMemberVariable(CCObject * pTarget, const char* pMemberVariableName, CCNode * pNode)
{
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_Score", CCLabelTTF*, m_Score);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_MaxScore", CCLabelTTF*, m_MaxScore);
    CCB_MEMBERVARIABLEASSIGNER_GLUE(this, "m_TopName", CCLabelTTF*, m_TopName);
    
    loadTopRecord();
    
	return false;
}

SEL_MenuHandler ScoreDialog::onResolveCCBCCMenuItemSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

SEL_CCControlHandler ScoreDialog::onResolveCCBCCControlSelector(CCObject * pTarget, const char* pSelectorName)
{
    return NULL;
}

void ScoreDialog::loadTopRecord()
{
    m_MaxScore->setString("0");
    m_TopName->setString("");
    m_Score->setString("0");
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScoreDialog::doChangeScore), EVENT_SCORE_CHANGE, NULL);
}

void ScoreDialog::doChangeScore(CCObject* data)
{
    DialogEvent *event = (DialogEvent*) data;
    if (event->getkey() == "Score"){
        CCString *str = CCString::create(std::string(m_Score->getString()));
        int value = str->intValue();
        CCString *temp = CCString::createWithFormat("%d",event->getvalue()+value);
        m_Score->setString(temp->getCString());
    }else if (event->getkey() == "MaxScore"){
        CCString *str = CCString::create(std::string(m_MaxScore->getString()));
        int value = str->intValue();
        if (event->getvalue() > value){
            CCString *temp = CCString::createWithFormat("%d",event->getvalue());
            m_MaxScore->setString(temp->getCString());
        }
    }else if (event->getkey() == "TopName"){
        m_TopName->setString(event->getstrvalue().c_str());
    }
}
