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

class DialogManager
{
public:
    DialogManager(void);
    ~DialogManager(void);

    bool init(CCLayer* owner);
	static DialogManager* create(CCLayer* owner);
    
    BasicDialog* getDialog(std::string dialogName);
    BasicDialog* showDialog(std::string dialogName, CCNode* pParent);
    BasicDialog* createDialog(std::string dialogName);
    void hideDialog(std::string dialogName);
private:
    CCLayer* m_pOwner;
    std::map<std::string, BasicDialog*> m_mDialogList;
};

#endif /* defined(__pball__DialogManager__) */
