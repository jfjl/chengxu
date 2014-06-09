//
//  DialogManager.cpp
//  pball
//
//  Created by sora on 13-4-17.
//
//

#include "DialogManager.h"
#include "ScoreDialog.h"
#include "CongratulationDialog.h"

DialogManager::DialogManager()
{
    
}

DialogManager::~DialogManager()
{
    
}

bool DialogManager::init(CCLayer* owner)
{
    m_pOwner = owner;
    m_mDialogList.clear();
/*
    ScoreDialog *scoreDialog = ScoreDialog::create();
    scoreDialog->setVisible(false);
    m_Owner->addChild(scoreDialog);
    m_DialogList->setObject(scoreDialog, "ScoreDialog");
 */
    return true;
}

DialogManager* DialogManager::create(CCLayer* owner)
{
    DialogManager* pRet = new DialogManager();
    pRet->init(owner);
    return pRet;
}

BasicDialog* DialogManager::createDialog(std::string dialogName)
{
    if (dialogName == "ScoreDialog")
    {
        return ScoreDialog::create();
    }
    
    return NULL;
}

BasicDialog* DialogManager::getDialog(std::string dialogName)
{
    std::map<std::string,BasicDialog*>::iterator iter = m_mDialogList.find(dialogName);
    if (iter == m_mDialogList.end()) return NULL;
        
    return iter->second;
}

BasicDialog* DialogManager::showDialog(std::string dialogName, CCNode* pParent)
{
    BasicDialog* pnode = getDialog(dialogName);
    if (! pnode)
    {
        pnode = createDialog(dialogName);
        
        if (! pnode) return NULL;
    };
    
    pnode->onShow(pParent);
    return pnode;
}

void DialogManager::hideDialog(std::string dialogName)
{
    BasicDialog* pnode = getDialog(dialogName);
    if (! pnode) return;
    
    pnode->onHide(true);
}