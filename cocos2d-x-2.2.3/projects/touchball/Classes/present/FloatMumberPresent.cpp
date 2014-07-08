//
//  FloatMumberPresent.cpp
//  touchball
//
//  Created by carlor on 14-7-2.
//
//

#include "FloatMumberPresent.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "DialogEvent.h"
#include "gameConst.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../ui/DialogEvent.h"
#include "../utils/gameConst.h"
#endif

FloatNumberPresent::FloatNumberPresent()
: m_lblNumber(NULL)
{
    
}

FloatNumberPresent::~FloatNumberPresent()
{
    
}

bool FloatNumberPresent::init(int id, std::string fileName, int imgWidth, int imgHeight, void* param)
{
    if (! BasicPresent::init(id, fileName, imgWidth, imgHeight)) {
        return false;
    }
    
    DialogEvent* pDialogEvent = (DialogEvent*) param;
    char buf[10] = {0};
    sprintf(buf, "%d", pDialogEvent->getvalue());
    
    m_lblNumber = ui::LabelAtlas::create();
    m_lblNumber->setProperty(buf, fileName, imgWidth, imgHeight, "0");
    this->addChild(m_lblNumber);
    
    return true;
}

FloatNumberPresent* FloatNumberPresent::create(int id, std::string fileName, int imgWidth, int imgHeight, void* param)
{
    FloatNumberPresent* pRet = new FloatNumberPresent();
    if (pRet && pRet->init(id, fileName, imgWidth, imgHeight, param)) {
        return pRet;
    }
    delete pRet;
    return NULL;
}

void FloatNumberPresent::onMoveBy()
{
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_PRESENT_HIDE, (CCObject *)this);
}

void FloatNumberPresent::onShow(void* param)
{
    BasicPresent::onShow(param);
    this->setPosition(280, 890);

	CCActionInterval *actionMoveBy = CCMoveBy::create(1.0f, ccp(0, 50));
    CCEaseOut* easeOut = CCEaseOut::create(actionMoveBy, 2);
	CCFiniteTimeAction* actions= CCSequence::create(easeOut,
                                                    CCCallFunc::create(this,callfunc_selector(FloatNumberPresent::onMoveBy)),NULL);
    
	this->runAction(actions);
}

void FloatNumberPresent::onHide(void* param)
{
    BasicPresent::onHide(param);
}
