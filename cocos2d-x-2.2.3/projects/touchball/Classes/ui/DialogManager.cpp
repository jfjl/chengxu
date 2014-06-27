//
//  DialogManager.cpp
//  pball
//
//  Created by sora on 13-4-17.
//
//

#include "DialogManager.h"
#include "ScoreDialog.h"
#include "ScoreXDialog.h"
#include "LevelDialog.h"
#include "PropsDialog.h"

DialogManager* g_dialogManager = 0;

DialogManager::DialogManager()
{
    g_dialogManager = this;
}

DialogManager::~DialogManager()
{
    
}

bool DialogManager::init(CCLayer* owner)
{
    m_pOwner = owner;
    m_mDialogList.clear();

	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(DialogManager::onHideDialog), EVENT_DIALOG_HIDE, NULL);
    
    return true;
}

DialogManager* DialogManager::create(CCLayer* owner)
{
    DialogManager* pRet = new DialogManager();
    pRet->init(owner);
    return pRet;
}

BasicDialog* DialogManager::createDialog(const char* dialogName)
{
    BasicDialog* pDialog;
//    if (strcasecmp(dialogName, "ScoreDialog") == 0)
//    {
//        pDialog = ScoreDialog::create();
//    }
    if (strcasecmp(dialogName, "ScoreXDialog") == 0)
    {
        pDialog = ScoreXDialog::create();
    }
    else if (strcasecmp(dialogName, "LevelDialog") == 0)
    {
        pDialog = LevelDialog::create();
    }
    else if (strcasecmp(dialogName, "PropsDialog") == 0)
    {
        pDialog = PropsDialog::create();
    }
    if (pDialog)
    {
        pDialog->setName(dialogName);
        m_mDialogList[dialogName] = pDialog;
        CCLog("create dialog: %s", dialogName);
    }
    
    return pDialog;
}

BasicDialog* DialogManager::getDialog(const char* dialogName)
{
    std::map<std::string,BasicDialog*>::iterator iter = m_mDialogList.find(dialogName);
    if (iter == m_mDialogList.end()) return NULL;
        
    return iter->second;
}

BasicDialog* DialogManager::showDialog(const char* dialogName, CCNode* pParent)
{
    BasicDialog* pnode = getDialog(dialogName);
    if (! pnode)
    {
        pnode = createDialog(dialogName);
        
        if (! pnode)
        {
            CCLog("create dialog failed: %s", dialogName);
            return NULL;
            
        }
    };
    
    if (! pParent) {
        pParent = this;
    }
    pnode->onShow(pParent);
    return pnode;
}

void DialogManager::hideDialog(const char* dialogName)
{
    BasicDialog* pnode = getDialog(dialogName);
    if (! pnode) return;
    
    pnode->onHide(true);
}

void DialogManager::onHideDialog(BasicDialog* pDialog)
{
    hideDialog(pDialog->getName());
}
