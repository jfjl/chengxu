//
//  PropsManager.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include "PropsManager.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ClientData.h"
#include "SceneManager.h"
#include "GameScript.h"
#include "DialogEvent.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../datas/ClientData.h"
#include "../SceneManager.h"
#include "../../script/GameScript.h"
#include "../../ui/DialogEvent.h"
#endif

PropsManager::PropsManager()
: m_Owner(NULL)
{
    
}

PropsManager::~PropsManager()
{
    
}

void PropsManager::clear()
{
    m_PropsList.clear();
}

bool PropsManager::init(ballMap* owner, int width, int height)
{
    clear();
    m_Owner = owner;
	m_Width = width;
	m_Height = height;
    m_seed = 0;
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(PropsManager::onSelectProps), EVENT_SELECT_PROPS, NULL);
    
    return true;
}

PropsManager* PropsManager::create(ballMap* owner, int width, int height)
{
	PropsManager *pRet = new PropsManager();
	pRet->init(owner, width, height);
	return pRet;
}

int PropsManager::newId()
{
    return ++m_seed;
}

BasicProps* PropsManager::addProps(int classId)
{
    BasicProps* pRet = BasicProps::create(getOwner(), newId(), classId);
    if (pRet) {
        m_PropsList[pRet->getID()] = pRet;
//        deleteProps(1);
    }
    
    return pRet;
}

bool PropsManager::deleteProps(int id)
{
    PropsMap::iterator it = m_PropsList.find(id);
    if (it == m_PropsList.end()) {
        return false;
    }
    
    m_PropsList.erase(it);
    return true;
}


void PropsManager::show(int pos, int classId)
{
    BasicProps* pProps = addProps(classId);
    if (! pProps) return;
    
    pProps->show(pos);
}

void PropsManager::hide(int pos)
{
    for (PropsMap::iterator it = m_PropsList.begin(); it != m_PropsList.end(); it++) {
        if (it->second->getPos() != pos) continue;
        deleteProps(it->second->getID());
        break;
    }
}

void PropsManager::pick(int value)
{
//    const propsCfg* pPropsCfg = g_clientData->getPropsCfg(value);
//    if (! pPropsCfg) return;
    
    m_SelectId = value;
    m_SelectPos = -1;
    
    const propsCfg* pPropsCfg = g_clientData->getPropsCfg(m_SelectId);
    if (! pPropsCfg) return;
    if (! pPropsCfg->OnPick) return;
    EventMap::iterator it = pPropsCfg->pEventList->mEventFuncs.find("OnPick");
    if (it == pPropsCfg->pEventList->mEventFuncs.end()) return;
    
    SceneData* pSceneData = g_sceneManager->getCurScene();
    g_gameScript->excuteFunction(it->second.key.c_str(), this, pSceneData, getOwner());
}

void PropsManager::put(int pos)
{
    if (m_SelectId == 0) return;
    
    const propsCfg* pPropsCfg = g_clientData->getPropsCfg(m_SelectId);
    if (! pPropsCfg) return;
    if (! pPropsCfg->OnPut) return;
    EventMap::iterator it = pPropsCfg->pEventList->mEventFuncs.find("OnPut");
    if (it == pPropsCfg->pEventList->mEventFuncs.end()) return;
    
    m_SelectPos = pos;
    
    SceneData* pSceneData = g_sceneManager->getCurScene();
    g_gameScript->excuteFunction(it->second.key.c_str(), this, pSceneData, getOwner());
}

void PropsManager::onRemoveBall(int pos)
{
    for (PropsMap::iterator it = m_PropsList.begin(); it != m_PropsList.end(); it++) {
        if (it == m_PropsList.end()) continue;
        it->second->onRemoveBall(pos);
    }
}

void PropsManager::onSelectProps(CCObject* pEvent)
{
    DialogEvent* pDialogEvent = (DialogEvent*) pEvent;

    pick(pDialogEvent->getvalue());
}

int PropsManager::script_getSelectPos(lua_State* L)
{
    lua_pushinteger(L, m_SelectPos);
    return 1;
}

int PropsManager::callFunction(lua_State* L)
{
	const char* funcName = lua_tostring(L, 2);
	if (strcmp(funcName, "getSelectPos") == 0)
		return script_getSelectPos(L);
    return 1;
}

