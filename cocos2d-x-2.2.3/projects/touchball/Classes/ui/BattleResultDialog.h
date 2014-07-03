//
//  BattleResultDialog.h
//  touchball
//
//  Created by carlor on 14-7-3.
//
//

#ifndef __touchball__BattleResultDialog__
#define __touchball__BattleResultDialog__

#include "BasicDialog.h"

class BattleResultDialog : public BasicDialog
{
public:
    BattleResultDialog();
    ~BattleResultDialog();
    
    CCB_STATIC_NEW_AUTORELEASE_OBJECT_WITH_INIT_METHOD(BattleResultDialog, create);
    
    void onShow(void* param);
    
protected:
    void initDialog();
    bool initInterface(ui::Layout* dialogUI);
private:
    void drawResult(int result, int score, int star);
    void onClickBackgroud(CCObject* sender,  ui::TouchEventType type);
    
};

#endif /* defined(__touchball__BattleResultDialog__) */
