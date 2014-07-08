//
//  LevelDialog.h
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#ifndef __touchball__LevelDialog__
#define __touchball__LevelDialog__

#include "BasicDialog.h"

struct levelInfo
{
    int level;
    int score;
    int star;
};

typedef std::vector<levelInfo> LevelInfoVector;

class LevelDialog : public BasicDialog
{
public:
    LevelDialog();
    ~LevelDialog();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(LevelDialog, create);
protected:
    void initDialog();
    bool initInterface(ui::Layout* dialogUI);
private:
    void buildLevelInfo();
    void setCurPage(int value);
    void drawLevel(ui::Layout* pParent, int index, int level);
    void drawLevelInfo(ui::Button* btnLevel, int level);
    void drawLock(ui::Button* btnLevel, int level);
    void drawOpen(ui::Button* btnLevel, int level);
    void drawOver(ui::Button* btnLevel, int level);
    
    void onClickLevel(CCObject* sender, ui::TouchEventType type);
    void onClickPageDown(CCObject* sender, ui::TouchEventType type);
    void onClickPageUp(CCObject* sender, ui::TouchEventType type);
    
    
private:
    LevelInfoVector m_vLevelInfo;
    int             m_nCurPage;
    int             m_nPageSize;
    int             m_nMaxPage;
};

#endif /* defined(__touchball__LevelDialog__) */
