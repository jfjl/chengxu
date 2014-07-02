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

class DialogManager : public CCLayer
{
public:
    DialogManager(void);
    ~DialogManager(void);

    virtual void onEnter();
    virtual void onExit();
    bool init(CCLayer* owner);
	static DialogManager* create(CCLayer* owner);
    
    BasicDialog* getDialog(const char* dialogName);
    BasicDialog* showDialog(const char* dialogName, void* param=NULL);
    void hideDialog(const char* dialogName, void* param=NULL);
    BasicDialog* createDialog(const char* dialogName);
    void removeDialog(const char* dialogName);
    
    void onUpdate(float dt);

    CC_SYNTHESIZE_READONLY(ui::TouchGroup*, m_pUILayer, UILayer);
private:
    CCLayer* m_pOwner;
    std::map<std::string, BasicDialog*> m_mDialogList;
};

extern DialogManager* g_dialogManager;

#endif /* defined(__pball__DialogManager__) */
