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

void PropsDialog::initDialog()
{
    BasicDialog::initDialog();
    m_pDialog = dynamic_cast<ui::Layout*>(GUIReader::shareReader()->widgetFromJsonFile("PropsDialog_1.json"));
    initInterface(getDialog());
}

bool PropsDialog::initInterface(ui::Layout* dialogUI)
{
    ui::Layout* backpnl = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(dialogUI, "Panel_4" ));
    if (! backpnl) {
        CCLOG("not find background");
        return false;
    }
    ui::Layout* back = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(backpnl, "pnlBack" ));
    if (! back) {
        CCLOG("not find back");
        return false;
    }
    ui::ImageView* imgback = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(back, "imgBack" ));
    if (! back) {
        CCLOG("not find back img");
        return false;
    }
    
    m_btnArray = CCArray::create();
    for (int i = 1001; i <= 1005; i++)
    {
        char buf[6];
        sprintf(buf, "props_%d.jpg", i);
        ui::ImageView* props = ui::ImageView::create();
        props->loadTexture(buf);
        props->setAnchorPoint(ccp(0, 0));
        props->ignoreContentAdaptWithSize(false);
        m_btnArray->addObject(props);
        
        sprintf(buf, "pnlProps%d", i-1001);
        ui::Layout* pnlProps = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(imgback, buf ));
        pnlProps->setTag(i);
        CCSize size = pnlProps->getSize();
        props->setSize(size);
        pnlProps->addTouchEventListener(this, ui::SEL_TouchEvent(&PropsDialog::onClickProps));
        pnlProps->addChild(props);
    }
    return true;
}

void PropsDialog::onClickProps(CCObject* sender,  ui::TouchEventType type)
{
    if (type != ui::TOUCH_EVENT_ENDED) return;
    ui::Layout* pnlProps = dynamic_cast<ui::Layout*>(sender);
    int index = pnlProps->getTag();

    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->setkey("props");
    pDialogEvent->setvalue(index);
    pDialogEvent->autorelease();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SELECT_PROPS, (CCObject*)pDialogEvent);
}