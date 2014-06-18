//
//  DialogManager.h
//  pball
//
//  Created by sora on 13-4-17.
//
//

#ifndef __pball__DialogManager__
#define __pball__DialogManager__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "BasicDialog.h"
using namespace cocos2d;
using namespace extension;

class DialogManager : public BasicObject, public CCObject
{
public:
    DialogManager(void);
    ~DialogManager(void);

    bool init(CCLayer* owner);
	static DialogManager* create(CCLayer* owner);
    
    BasicDialog* getDialog(const char* dialogName);
    BasicDialog* showDialog(const char* dialogName, CCNode* pParent);
    BasicDialog* createDialog(const char* dialogName);
    void hideDialog(const char* dialogName);
private:
    void onHideDialog(BasicDialog* pDialog);
private:
    CCLayer* m_pOwner;
    std::map<std::string, BasicDialog*> m_mDialogList;
};

extern DialogManager* g_dialogManager;

#endif /* defined(__pball__DialogManager__) */
