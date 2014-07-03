//
//  BattleResultDialog.cpp
//  touchball
//
//  Created by carlor on 14-7-3.
//
//

#include "BattleResultDialog.h"

BattleResultDialog::BattleResultDialog()
{
    
}

BattleResultDialog::~BattleResultDialog()
{
    
}


void BattleResultDialog::initDialog()
{
    BasicDialog::initDialog();
    m_pDialog = dynamic_cast<ui::Layout*>(GUIReader::shareReader()->widgetFromJsonFile("BattleResultDialog_1.json"));
    initInterface(getDialog());
}

bool BattleResultDialog::initInterface(ui::Layout* dialogUI)
{
    ui::Layout* back = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(dialogUI, "back" ));
    if (! back) {
        CCLOG("not find back");
        return false;
    }
    back->addTouchEventListener(this, ui::SEL_TouchEvent(&BattleResultDialog::onClickBackgroud));

    return true;
}

void BattleResultDialog::drawResult(int result, int score, int star)
{
    ui::Layout* back = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(getDialog(), "back" ));
    if (! back) {
        CCLOG("not find back");
        return;
    }
    ui::Label* lblResult = static_cast<ui::Label*>(ui::UIHelper::seekWidgetByName(back, "lblResult" ));
    if (lblResult) {
        lblResult->setText(result ? "过关了" : "失败了");
    }
    ui::Label* lblScore = static_cast<ui::Label*>(ui::UIHelper::seekWidgetByName(back, "lblScore" ));
    if (lblScore) {
        char buf[10] = {0};
        sprintf(buf, "%d", score);
        lblScore->setText(buf);
    }
    ui::Label* lblStar = static_cast<ui::Label*>(ui::UIHelper::seekWidgetByName(back, "lblStar" ));
    if (lblStar) {
        char buf[10] = {0};
        sprintf(buf, "%d", star);
        lblStar->setText(buf);
    }
}

void BattleResultDialog::onShow(void *param)
{
    BasicDialog::onShow(param);
    
    DialogEvent* pDialogEvent = (DialogEvent*) param;
    drawResult(pDialogEvent->getIntValue("result"), pDialogEvent->getIntValue("score"), pDialogEvent->getIntValue("star"));
}

void BattleResultDialog::onClickBackgroud(CCObject* sender,  ui::TouchEventType type)
{
    if (type != ui::TOUCH_EVENT_ENDED) return;
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ENTER_MAINMENU, (CCObject *)this);
}

