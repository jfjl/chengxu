//
//  ScoreDialog.h
//  pball
//
//  Created by sora on 13-4-17.
//
//

#ifndef __pball__ScoreDialog__
#define __pball__ScoreDialog__

#include "BasicDialog.h"


class ScoreDialog : public BasicDialog
{
public:
    ScoreDialog();
    ~ScoreDialog();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(ScoreDialog, create);
    
    void onShow(void* param);
    bool onUpdate(float dt);
protected:
    void initDialog();
    bool initInterface(ui::Layout* dialogUI);
private:
    void drawLevelScore(int level);
    void drawCurScore();
    void setToScore(int score);
    int getChangeScore();
    void onChangeScore(CCObject* data);
private:
    ui::LabelAtlas* m_lblNeedScore;
    ui::LabelAtlas* m_lblCurScore;
    ui::LoadingBar* m_proCurScore;
    int m_nCurLevel;
    int m_nMaxScore;
    int m_nCurScore;
    int m_nToScore;
    bool m_bInit;
};

#endif /* defined(__pball__ScoreDialog__) */
