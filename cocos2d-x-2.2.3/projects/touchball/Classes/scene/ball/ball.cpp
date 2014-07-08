//
//  ball.cpp
//  ppball
//
//  Created by sora on 13-3-13.
//  Copyright __MyCompanyName__ 2013年. All rights reserved.
//

#include "ball.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ClientData.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../datas/ClientData.h"
#endif

ball::ball(void)
: m_pos(CCPointZero)
, m_ID(-1)
, m_ClassID(-1)
, m_BallState(0)
, m_StateSprite(NULL)
, m_pLabel(NULL)
{
}


ball::~ball(void)
{
	std::cout<<this->m_uReference<<std::endl;
}

CCString* ball::getImage(int classId)
{
    int imageId = (int) (classId / 1000) - 1;
	CCString *imgName = CCString::createWithFormat("ball_%d.png", imageId);
    return imgName;
}

bool ball::init(int classID, CCPoint pos)
{
	CCString *imgName = getImage(classID);
	if (!CCSprite::initWithSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imgName->getCString()))){
		return false;
	}
    this->setScale(TEXTURESCALE);
    //this->setAnchorPoint(ccp(0, 0));
    
	m_pos = pos;
    m_BallState = BallStateNormal;
    
	imgName = CCString::createWithFormat("ball_state_%d.png", m_BallState);
	m_StateSprite = CCSprite::createWithSpriteFrameName(imgName->getCString());
    m_StateSprite->setScale(TEXTURESCALE);
    float s = MAPCELL_SIZE * TEXTURESCALE / 2;
    m_StateSprite->setPosition(ccp(s, s));
    //m_StateSprite->setAnchorPoint(ccp(0, 0));
	this->addChild(m_StateSprite);
    
    m_pLabel = CCLabelTTF::create("", "微软雅黑", 12);
    m_pLabel->setAnchorPoint(ccp(0, 0));
    this->addChild(m_pLabel);
    
	return true;
}

ball* ball::create(int classID, CCPoint pos)
{
	ball *pRet = new ball();
	pRet->init(classID, pos);
	pRet->autorelease();
	return pRet;
}

void ball::setVisible(bool bVisible, bool bPlayAction, bool bAllAction)
{
    if (! bVisible && ! bAllAction)
    {
        const ballCfg* pBallCfg = g_clientData->getBallCfg(m_ClassID);
        if (pBallCfg && pBallCfg->PreBall)
        {
            setBallClass(pBallCfg->PreBall);
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ACTION_HIDECOMPLETE, (CCObject*)this);
            return;
        }
    }
    
	if (this->isVisible() == bVisible) return;
    
	CCSprite::setVisible(bVisible);
    
	if (bVisible){
		if (bPlayAction)
			playShow();
		else{
			setBallState(BallStateNormal);
		}
	}else if (! bVisible && bPlayAction){
		if (bPlayAction)
			playHide();
		else{
			setBallState(BallStateHide);
		}
	}
}

void ball::setBallState(BallStates value)
{
	if (m_BallState == value) return;
	m_BallState = value;
	if (m_BallState > 3) return;

	CCString *imgName = CCString::createWithFormat("ball_state_%d.png", m_BallState);
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imgName->getCString());
	if (frame != NULL)
		m_StateSprite->setDisplayFrame(frame);
}

void ball::setBallClass(int value)
{
	if (value == m_ClassID) return;
	
	m_ClassID = value;
	//if (value > 6) return;
	CCString *imgName = getImage(m_ClassID);
    CCSpriteFrame *frame = CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(imgName->getCString());
	if (frame != NULL)
		this->setDisplayFrame(frame);
    
    setBallAddition();

}

void ball::setBallAddition()
{
    m_pLabel->setString("");
    
    const ballCfg* pBallCfg = g_clientData->getBallCfg(m_ClassID);
    char s[32];
    
    switch (pBallCfg->BallType) {
        case BALLTYPE_ROW:
            if (! pBallCfg->CanMove)
            {
                sprintf(s, "r nm %d", pBallCfg->nRemoveCount);
            }
            else
            {
                sprintf(s, "r %d", pBallCfg->nRemoveCount);
            }
            break;
        case BALLTYPE_COL:
            if (! pBallCfg->CanMove)
            {
                sprintf(s, "c nm %d", pBallCfg->nRemoveCount);
            }
            else
            {
                sprintf(s, "c %d", pBallCfg->nRemoveCount);
            }
            break;
        case BALLTYPE_X:
            if (! pBallCfg->CanMove)
            {
                sprintf(s, "x nm %d", pBallCfg->nRemoveCount);
            }
            else
            {
                sprintf(s, "x %d", pBallCfg->nRemoveCount);
            }
            break;
        case BALLTYPE_AROUND:
            if (! pBallCfg->CanMove)
            {
                sprintf(s, "a nm %d", pBallCfg->nRemoveCount);
            }
            else
            {
                sprintf(s, "a %d", pBallCfg->nRemoveCount);
            }
            break;
        case BALLTYPE_SAME:
            if (! pBallCfg->CanMove)
            {
                sprintf(s, "s nm %d", pBallCfg->nRemoveCount);
            }
            else
            {
                sprintf(s, "s %d", pBallCfg->nRemoveCount);
            }
            break;
            
        default:
            if (! pBallCfg->CanMove)
            {
                sprintf(s, "nm %d", pBallCfg->nRemoveCount);
            }
            else
            {
                sprintf(s, "%d", pBallCfg->nRemoveCount);
            }
            break;
    }
    
    m_pLabel->setString(s);
}

void ball::onShow()
{
	//CCSprite::setVisible(true);
	this->setScale(TEXTURESCALE);
	this->setBallState(BallStateNormal);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ACTION_SHOWCOMPLETE, (CCObject *)this);
}

void ball::playShow()
{
	this->setBallState(BallStatePlayShow);
	this->setScale(0.0);
	
	CCActionInterval *actionScale = CCScaleTo::create(0.5f, TEXTURESCALE);
	CCFiniteTimeAction* actions= CCSequence::create(actionScale,
                                                    CCCallFunc::create(this,callfunc_selector(ball::onShow)),NULL);
    
	this->runAction(actions);
}

void ball::onHide()
{
	//CCSprite::setVisible(false);
	this->setScale(TEXTURESCALE);
	this->setBallState(BallStateHide);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ACTION_HIDECOMPLETE, (CCObject*)this);
}

void ball::playHide()
{    
	this->setBallState(BallStatePlayDelete);
	this->setScale(TEXTURESCALE);

	CCBlink *actionBlink = CCBlink::create(0.5f, 3);
	CCActionInterval *actionInterval = CCScaleTo::create(0.5f, 0.0);
	CCFiniteTimeAction* actions= CCSequence::create(actionBlink, actionInterval,
                                                    CCCallFunc::create(this,callfunc_selector(ball::onHide)),NULL);
    
	this->runAction(actions);
}