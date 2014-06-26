//
//  LevelDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include "LevelDialog.h"
#include "ClientData.h"

LevelDialog::LevelDialog()
: m_edtLevel(NULL)
, m_btnConfirm(NULL)
{
    
}

LevelDialog::~LevelDialog()
{
//    CC_SAFE_RELEASE(m_edtLevel);
//    CC_SAFE_RELEASE(m_btnConfirm);
}

bool LevelDialog::init()
{
    if (! BasicDialog::init()) return false;
    
    m_edtLevel = CCEditBox::create(CCSizeMake(200, 40), CCScale9Sprite::create("edt_input_01.png"));
    m_edtLevel->setPosition(ccp(0, 0));
    m_edtLevel->setAnchorPoint(ccp(0, 0.5));
    m_edtLevel->setFont("", 25);
    m_edtLevel->setInputMode(kEditBoxInputModeNumeric);
    m_edtLevel->setMaxLength(2);
    m_edtLevel->setText("1");
    m_edtLevel->setColor(ccc3(255, 255, 255));
    this->addChild(m_edtLevel);
    
    CCScale9Sprite * btnNormal = CCScale9Sprite::create("btn_confirm_normal_01.png");
    CCScale9Sprite * btnDown = CCScale9Sprite::create("btn_confirm_down_01.png");
    
    m_btnConfirm = CCControlButton::create(btnNormal);
    m_btnConfirm->setBackgroundSpriteForState(btnDown, CCControlStateSelected);
    m_btnConfirm->setAnchorPoint(ccp(0, 0.5f));
    m_btnConfirm->setContentSize(btnNormal->getOriginalSize());
    m_btnConfirm->setPreferredSize(btnNormal->getOriginalSize());
    
    m_btnConfirm->setPosition(ccp(230, 0));
    m_btnConfirm->addTargetWithActionForControlEvents(this, cccontrol_selector(LevelDialog::onClickConfirm), CCControlEventTouchUpInside);
    this->addChild(m_btnConfirm);
    
    return true;
}

void LevelDialog::onShow(CCNode* pParent)
{
    BasicDialog::onShow(pParent);
    this->setPosition(ccp(50, 20));
}

void LevelDialog::editBoxReturn(CCEditBox* editBox)
{
    
}

void LevelDialog::onClickConfirm(CCObject * sender, CCControlEvent controlEvent)
{
    int level = (atoi)(m_edtLevel->getText());
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(level);
    if (! pLevelCfg)
    {
        return;
    }
    
    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->setkey("level");
    pDialogEvent->setvalue(level);
    pDialogEvent->autorelease();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ENTER_GAME, (CCObject*)pDialogEvent);    
}


