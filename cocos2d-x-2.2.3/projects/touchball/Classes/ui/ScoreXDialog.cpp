//
//  ScoreXDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-16.
//
//

#include "ScoreXDialog.h"

ScoreXDialog::ScoreXDialog()
{
    
}

ScoreXDialog::~ScoreXDialog()
{
    CC_SAFE_DELETE(m_lblStep);
}

bool ScoreXDialog::init()
{
    if (! BasicDialog::init()) return false;
    
    m_lblStep = CCLabelTTF::create("0", "微软雅黑", 12);
    m_lblStep->setAnchorPoint(ccp(0, 0));
    this->addChild(m_lblStep);
    
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScoreXDialog::onBallMove),
                                                                  EVENT_BALL_MOVE, NULL);
    
    return true;
}

void ScoreXDialog::onShow(CCNode* pParent)
{
    BasicDialog::onShow(pParent);
    this->setPosition(ccp(360, 160));
}

void ScoreXDialog::onBallMove(CCObject* pObj)
{
    int nStep = atoi(m_lblStep->getString());
    nStep++;
    
    char sStep[10] = {0};
    sprintf(sStep, "%d", nStep);
    m_lblStep->setString(sStep);
}


