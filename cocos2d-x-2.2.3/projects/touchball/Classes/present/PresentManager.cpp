//
//  PresentManager.cpp
//  touchball
//
//  Created by carlor on 14-7-2.
//
//

#include "gameConst.h"
#include "PresentManager.h"
#include "FloatMumberPresent.h"
#include "DialogEvent.h"

PresentManager* g_presentManager = 0;

PresentManager::PresentManager()
: m_pOwner(NULL)
, m_mPresentList(NULL)
{
    CCAssert(g_presentManager == NULL, "重复创建 presentManager");
    g_presentManager = this;
}

PresentManager::~PresentManager()
{
    m_mPresentList->removeAllObjects();
}

void PresentManager::clear()
{
}

int PresentManager::getSeed()
{
    return ++m_nSeed;
}

bool PresentManager::init(CCLayer* owner)
{
    if (! CCLayer::init()) {
        return false;
    }
    
    m_pOwner = owner;
    m_mPresentList = CCDictionary::create();
    m_mPresentList->retain();
    
    m_nSeed = 0;
    
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PresentManager::onChangeScore), EVENT_SCORE_CHANGE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PresentManager::onHidePresent), EVENT_PRESENT_HIDE, NULL);

    return true;
}

PresentManager* PresentManager::create(CCLayer* owner)
{
    PresentManager* pRet = new PresentManager();
    if (pRet && pRet->init(owner)) {
        pRet->autorelease();
        return pRet;
    }
    
    delete pRet;
    return NULL;
}

void PresentManager::onEnter()
{
    CCLayer::onEnter();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ENTER_PRESENTLAYER, (CCObject*)this);
}

void PresentManager::onExit()
{
    CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_EXIT_PRESENTLAYER, (CCObject*)this);
}

void PresentManager::onUpdate(float dt)
{
	CCDictElement *pElement;
	CCDICT_FOREACH(m_mPresentList, pElement) {
		BasicPresent* pPresent = (BasicPresent*) pElement->getObject();
        pPresent->onUpdate(dt);
    }
}


BasicPresent* PresentManager::getPresent(int id)
{
    CCObject *obj = m_mPresentList->objectForKey(id);
    if (! obj) {
        return NULL;
    }
    
    return (BasicPresent*) obj ;
}

BasicPresent* PresentManager::addPresent(PresentType type, std::string sFileName, int imgWidth, int imgHeight, void* param)
{
    BasicPresent* pRet;
    switch (type) {
        case PresentType_FloatNumber:
            pRet = FloatNumberPresent::create(getSeed(), sFileName, imgWidth, imgHeight, param);
            break;
        default:
            break;
    }
    if (! pRet) return NULL;
    m_mPresentList->setObject(pRet, pRet->getID());
    pRet->onShow(NULL);
    this->addChild(pRet);
    
    return pRet;
}

bool PresentManager::removePresent(int id)
{
    BasicPresent* pPresent = getPresent(id);
    if (! pPresent) {
        return false;
    }
    
    pPresent->onHide(NULL);
    m_mPresentList->removeObjectForKey(id);
    
    return true;
}

//////
void PresentManager::onHidePresent(CCObject* object)
{
    BasicPresent* pPresent = (BasicPresent*) object;
    removePresent(pPresent->getID());
}

void PresentManager::onChangeScore(CCObject* data)
{
    addPresent(PresentType_FloatNumber, "img_number_04.png", 20, 30, data);
}

