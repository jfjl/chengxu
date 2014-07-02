//
//  DialogManager.cpp
//  pball
//
//  Created by sora on 13-4-17.
//
//

#include "DialogManager.h"
#include "ScoreDialog.h"
#include "LevelDialog.h"
#include "PropsDialog.h"

DialogManager* g_dialogManager = 0;

DialogManager::DialogManager()
{
    CCAssert(g_dialogManager == NULL, "重复创建 dialogManager");
    g_dialogManager = this;
}

DialogManager::~DialogManager()
{
    
}

void DialogManager::onEnter()
{
    CCLayer::onEnter();
    m_pUILayer = ui::TouchGroup::create();
    m_pUILayer->scheduleUpdate();
    this->addChild(m_pUILayer);
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ENTER_DIALOGLAYER, (CCObject*)this);
}

void DialogManager::onExit()
{
    CCLayer::onExit();
    m_pUILayer->removeFromParent();
    SceneReader::sharedSceneReader()->purge();
    GUIReader::shareReader()->purge();
    cocos2d::extension:: ActionManager ::shareManager()->purge();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_EXIT_DIALOGLAYER, (CCObject*)this);
}

bool DialogManager::init(CCLayer* owner)
{
    m_pOwner = owner;
    m_mDialogList.clear();
    
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
    if (strcasecmp(dialogName, "ScoreDialog") == 0)
    {
        pDialog = ScoreDialog::create();
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

void DialogManager::removeDialog(const char* dialogName)
{
    std::map<std::string,BasicDialog*>::iterator iter = m_mDialogList.find(dialogName);
    if (iter == m_mDialogList.end()) return;
    
    m_mDialogList.erase(iter);
}

BasicDialog* DialogManager::getDialog(const char* dialogName)
{
    std::map<std::string,BasicDialog*>::iterator iter = m_mDialogList.find(dialogName);
    if (iter == m_mDialogList.end()) return NULL;
        
    return iter->second;
}

BasicDialog* DialogManager::showDialog(const char* dialogName, void* param)
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
    
    if (pnode->getDialog()) {
        pnode->onShow(param);
        pnode->retain();
        getUILayer()->addWidget(pnode->getDialog());
    }
    return pnode;
}

void DialogManager::hideDialog(const char* dialogName, void* param)
{
    BasicDialog* pnode = getDialog(dialogName);
    if (! pnode) return;
    
    if (pnode->getDialog()) {
        pnode->onHide(param);
        removeDialog(dialogName);
        getUILayer()->removeWidget(pnode->getDialog());
        pnode->release();
    }
}

void DialogManager::onUpdate(float dt)
{
    for (std::map<std::string,BasicDialog*>::iterator iter = m_mDialogList.begin(); iter != m_mDialogList.end(); iter++) {
        iter->second->onUpdate(dt);
    }
}

