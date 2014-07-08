//
//  ScoreDialog.cpp
//  pball
//
//  Created by sora on 13-4-17.
//
//

#include "ScoreDialog.h"
#include "DialogEvent.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ClientData.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../datas/ClientData.h"
#endif

ScoreDialog::ScoreDialog()
{
    CCLOG("CREATE SCOREDIALOG");
    
}

ScoreDialog::~ScoreDialog()
{
    
}

void ScoreDialog::initDialog()
{
    BasicDialog::initDialog();
    m_pDialog = dynamic_cast<ui::Layout*>(GUIReader::shareReader()->widgetFromJsonFile("ScoreDialog_1.json"));
    initInterface(getDialog());
}

bool ScoreDialog::initInterface(ui::Layout* dialogUI)
{
    ui::Layout* pnlBack = static_cast<ui::Layout*>(ui::UIHelper::seekWidgetByName(dialogUI, "pnlBack" ));
    if (! pnlBack) {
        CCLOG("not find background");
        return false;
    }
    ui::ImageView* imgBack = static_cast<ui::ImageView*>(ui::UIHelper::seekWidgetByName(pnlBack, "imgBack" ));
    if (! imgBack)
    {
        CCLOG("not find imgback");
        return false;
    }
    m_proCurScore = static_cast<ui::LoadingBar*>(ui::UIHelper::seekWidgetByName(imgBack, "proScore"));
    if (! m_proCurScore) {
        CCLOG("not find proscore");
        return false;
    }
    m_lblNeedScore = static_cast<ui::LabelAtlas*>(ui::UIHelper::seekWidgetByName(imgBack, "lblNeedScore"));
    m_lblCurScore = static_cast<ui::LabelAtlas*>(ui::UIHelper::seekWidgetByName(m_proCurScore, "lblCurScore"));
    
    
    m_nRefreshTime = 0.001f;

    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScoreDialog::onChangeScore), EVENT_SCORE_CHANGE, NULL);
    CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(ScoreDialog::onChangeStep), EVENT_STEP_CHANGE, NULL);
    
    return true;
}

void ScoreDialog::drawLevelScore(int score)
{
    m_nMaxScore = score;

    m_bInit = false;
    m_nCurScore = m_nMaxScore;
    m_nToScore = 0;
    drawCurScore();
}

void ScoreDialog::setToScore(int score)
{
    m_nToScore = m_nCurScore + score;
}

void ScoreDialog::drawCurScore()
{
    char bufScore[10] = {0};
    sprintf(bufScore, "%d", m_nCurScore);
    m_lblCurScore->setStringValue(bufScore);

    int percent = ((m_nMaxScore - (m_nMaxScore - m_nCurScore)) * 100) / 100;
    m_proCurScore->setPercent(percent);
}

void ScoreDialog::drawCurStep(int step)
{
    char bufstep[10] = {0};
    sprintf(bufstep, "%d", step);
    m_lblNeedScore->setStringValue(bufstep);
    
}

int ScoreDialog::getChangeScore()
{
    int t = abs(m_nCurScore - m_nToScore);
    int v = 0;
    if (t > 100) {
        v = t;
    }else if (t > 10) {
        v = 10;
    }else{
        v = 1;
    }
    if (m_nCurScore > m_nToScore) {
        return -v;
    }else{
        return v;
    }
}

bool ScoreDialog::onUpdate(float dt)
{
    bool result = BasicDialog::onUpdate(dt);
    if (! result) return result;

    if (m_nCurScore == m_nToScore) {
        if (! m_bInit) {
            DialogEvent* pDialogEvent = new DialogEvent();
            pDialogEvent->setkey("level");
            pDialogEvent->setvalue(m_nCurLevel);
            pDialogEvent->autorelease();
            
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_START_LEVEL, (CCObject *)pDialogEvent);
            
            m_bInit = true;
        }
        return result;
    }
    
    m_nCurScore += getChangeScore();
    drawCurScore();
    
    return result;
}

void ScoreDialog::onShow(void* param)
{
    BasicDialog::onShow(param);
    DialogEvent* pDialogEvent = (DialogEvent* ) param;
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(pDialogEvent->getvalue());
    if (! pLevelCfg) return;
    
    m_nCurLevel = pLevelCfg->Level;
    drawLevelScore(pLevelCfg->Score1);
    drawCurStep(pLevelCfg->MaxStep);
    getDialog()->setPosition(ccp(0, 720));
}

void ScoreDialog::onChangeScore(CCObject* data)
{
    DialogEvent* pDialogEvent = (DialogEvent*) data;
    setToScore(pDialogEvent->getvalue());
}

void ScoreDialog::onChangeStep(CCObject* data)
{
    DialogEvent* pDialogEvent = (DialogEvent*) data;
    drawCurStep(pDialogEvent->getvalue());
}
