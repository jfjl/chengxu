//
//  LevelDialog.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include "LevelDialog.h"
#include "ClientData.h"
#include "UserLocalData.h"

LevelDialog::LevelDialog()
{
}

LevelDialog::~LevelDialog()
{
    m_vLevelInfo.clear();
}

void LevelDialog::initDialog()
{
    BasicDialog::initDialog();
    m_pDialog =dynamic_cast<ui::Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ChoiceLevelDialog_1.json"));
    initInterface(getDialog());
}

void LevelDialog::buildLevelInfo()
{
    m_vLevelInfo.clear();
    int maxLevel = UserLocalData::getMaxLevel();
    
    bool isOver = true;
    for (int i = 0; i < maxLevel; i++) {
        int score = 0;
        int star = 0;
        
        bool had = UserLocalData::getLevelInfo(i+1, score, star);
        
        levelInfo info;
        info.level = i + 1;
        info.score = score;
        info.star = star;

        m_vLevelInfo.push_back(info);
        if (i + 1 == maxLevel && had) {
            const levelCfg* plevelCfg = g_clientData->getLevelCfg(i + 1);
            if (! plevelCfg) continue;
            isOver = plevelCfg && plevelCfg->Score1 <= score && maxLevel + 1 <= g_clientData->getMaxLevel();
        }
    }
    
    if (isOver) {
        levelInfo info;
        info.level = maxLevel + 1;
        info.score = 0;
        info.star = 0;
        m_vLevelInfo.push_back(info);
    }
    
    
    
    int temp = m_vLevelInfo.size() % m_nPageSize != 0 ? 1 : 0;
    m_nMaxPage = m_vLevelInfo.size() / m_nPageSize + temp;
}

void LevelDialog::drawLock(ui::Button* btnLevel, int level)
{
    btnLevel->setVisible(strcasecmp(btnLevel->getName(), "btnLock") == 0);
}

void LevelDialog::drawOpen(ui::Button* btnLevel, int level)
{
    btnLevel->setVisible(strcasecmp(btnLevel->getName(), "btnOpen") == 0);
    if (! btnLevel->isVisible()) return;
    
    char buflevel[4] = {0};
    sprintf(buflevel, "%d", level);
    string slevel = buflevel;

    ui::LabelAtlas* lblLevel = static_cast<ui::LabelAtlas*>(ui::UIHelper::seekWidgetByName(btnLevel, "lblLevel"));
    lblLevel->setStringValue(slevel);
}

void LevelDialog::drawOver(ui::Button* btnLevel, int level)
{
    btnLevel->setVisible(strcasecmp(btnLevel->getName(), "btnOver") == 0);
    
    if (! btnLevel->isVisible()) return;
    
    char buflevel[4] = {0};
    sprintf(buflevel, "%d", level);
    string slevel = buflevel;
    ui::LabelAtlas* lblLevel = static_cast<ui::LabelAtlas*>(ui::UIHelper::seekWidgetByName(btnLevel, "lblLevel"));
    lblLevel->setStringValue(slevel);

    const levelCfg* plevelCfg = g_clientData->getLevelCfg(level);
    
    ui::Layout* pnlFlower = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(btnLevel, "pnlFlower"));
    ui::ImageView* imgFlower0 = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(pnlFlower, "imgFlower0"));
    ui::ImageView* imgFlower1 = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(pnlFlower, "imgFlower1"));
    imgFlower0->setVisible(m_vLevelInfo[level-1].score >= plevelCfg->Score3);
    imgFlower1->setVisible(m_vLevelInfo[level-1].score < plevelCfg->Score3);
    
    ui::Layout* pnlStar = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(btnLevel, "pnlStar"));
    ui::ImageView* imgStar0 = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(pnlStar, "imgStar0"));
    ui::ImageView* imgStar1 = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(pnlStar, "imgStar1"));
    ui::ImageView* imgStar2 = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(pnlStar, "imgStar2"));
    imgStar0->setVisible(m_vLevelInfo[level-1].star == 1);
    imgStar1->setVisible(m_vLevelInfo[level-1].star == 2);
    imgStar2->setVisible(m_vLevelInfo[level-1].star == 3);
    
}

void LevelDialog::drawLevelInfo(ui::Button* btnLevel, int level)
{
    const levelCfg* plevelCfg = g_clientData->getLevelCfg(level);
    
    if (level > m_vLevelInfo.size() || plevelCfg == NULL) {
        drawLock(btnLevel, level);
    }else if (m_vLevelInfo[level-1].score < plevelCfg->Score1) {
        drawOpen(btnLevel, level);
    }else{
        drawOver(btnLevel, level);
    }
    
}

void LevelDialog::drawLevel(ui::Layout* pParent, int index, int level)
{
    char buf[10] = {0};
    sprintf(buf, "pnlLevel%d", index);
    ui::Layout* pnlLevel = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(pParent, buf));

    int maxLevel = g_clientData->getMaxLevel();
    if (level > maxLevel){
        pnlLevel->setVisible(false);
        return;
    }
    pnlLevel->setVisible(true);
    
    for (int i = 0; i < pnlLevel->getChildrenCount(); i++) {
        ui::Button* btnLevel = static_cast<ui::Button *>(pnlLevel->getChildren()->objectAtIndex(i));
        drawLevelInfo(btnLevel, level);
    }
}

void LevelDialog::setCurPage(int value)
{
    if (value == m_nCurPage) return;
    if (value <= 0 || value > m_nMaxPage) return;
    
    m_nCurPage = value;

    ui::Layout* back = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(getDialog(), "background" ));
    ui::Layout* backMiddle = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(back, "backMiddle" ));

    for (int i = 0; i < m_nPageSize; i++)
    {
        drawLevel(backMiddle, i, (m_nCurPage - 1) * m_nPageSize + i + 1);
    }
}

bool LevelDialog::initInterface(ui::Layout* dialogUI)
{
    ui::Layout* back = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(dialogUI, "background" ));
    if (! back) {
        CCLOG("not find background");
        return false;
    }
    ui::Layout* backMiddle = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(back, "backMiddle" ));
    if (! backMiddle)
    {
        CCLOG("not find backmiddle");
        return false;
    }
    
    buildLevelInfo();
    for (int i = 0; i < m_nPageSize; i++)
    {
        char buf[10] = {0};
        sprintf(buf, "pnlLevel%d", i);
        ui::Layout* pnlLevel = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(backMiddle, buf));
        pnlLevel->setTag(i+1);
        ui::Layout* pnlEvent = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(pnlLevel, "pnlEvent"));
        pnlEvent->setTag(i+1);
        pnlEvent->addTouchEventListener(this, ui::SEL_TouchEvent(&LevelDialog::onClickLevel));
    }
                                        
    ui::Button* btnPageDown = static_cast<ui::Button*>(ui::UIHelper::seekWidgetByName(backMiddle, "btnPageDown"));
    btnPageDown->addTouchEventListener(this, ui::SEL_TouchEvent(&LevelDialog::onClickPageDown));
    ui::Button* btnPageUp = static_cast<ui::Button*>(ui::UIHelper::seekWidgetByName(backMiddle, "btnPageUp"));
    btnPageUp->addTouchEventListener(this, ui::SEL_TouchEvent(&LevelDialog::onClickPageUp));
    
    
    setCurPage(1);

    return true;
}

void LevelDialog::onClickLevel(CCObject* sender, ui::TouchEventType type)
{
    if (type != ui::TOUCH_EVENT_ENDED) return;
    ui::Layout* pnlLevel = dynamic_cast<ui::Layout*>(sender);
    int index = pnlLevel->getTag();
    int level = (m_nCurPage - 1) * m_nPageSize + index;
    if (level > m_vLevelInfo.size()) return;
    
    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->setkey("level");
    pDialogEvent->setvalue(level);
    pDialogEvent->autorelease();
    
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ENTER_LEVEL, (CCObject *)pDialogEvent);
}

void LevelDialog::onClickPageDown(CCObject* sender, ui::TouchEventType type)
{
    setCurPage(m_nCurPage - 1);
}

void LevelDialog::onClickPageUp(CCObject* sender, ui::TouchEventType type)
{
    setCurPage(m_nCurPage + 1);
}


