//
//  PropsDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include "PropsDialog.h"

PropsDialog::PropsDialog()
: m_btnArray(NULL)
{
    
}

PropsDialog::~PropsDialog()
{
    m_btnArray->removeAllObjects();
    CC_SAFE_DELETE(m_btnArray);
}

bool PropsDialog::init()
{
    if (! BasicDialog::init()) return false;
    
    m_btnArray = CCArray::create();
    for (int i = 1001; i <= 1005; i++)
    {
        char buf[6];
        sprintf(buf, "%d", i);
        CCLabelTTF* lblCaption = CCLabelTTF::create(buf, "", 20);
        CCScale9Sprite* ssPrite = CCScale9Sprite::create("btn_confirm_normal_01.png");
        CCControlButton* btnProps = CCControlButton::create(lblCaption, ssPrite);
        btnProps->setContentSize(ssPrite->getContentSize());
        m_btnArray->addObject(btnProps);
        btnProps->addTargetWithActionForControlEvents(this, cccontrol_selector(PropsDialog::onClickProps), CCControlEventTouchUpInside);
        btnProps->setPosition(0, 40* (i - 1001));
        this->addChild(btnProps);
    }
    
    return true;
}

void PropsDialog::onShow(CCNode* pParent)
{
    BasicDialog::onShow(pParent);
    this->setPosition(ccp(450, 50));
}

void PropsDialog::onClickProps(CCObject * sender, CCControlEvent controlEvent)
{
    CCControlButton* btn = (CCControlButton* )sender;
    CCString* sCaption = btn->getTitleForState(CCControlStateNormal);

    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->setkey("props");
    pDialogEvent->setvalue(sCaption->intValue());
    pDialogEvent->autorelease();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SELECT_PROPS, (CCObject*)pDialogEvent);
}