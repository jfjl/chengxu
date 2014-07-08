//
//  BasicProps.cpp
//  touchball
//
//  Created by carlor on 14-6-17.
//
//

#include "BasicProps.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "GameScript.h"
#include "ClientData.h"
#include "SceneManager.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../script/GameScript.h"
#include "../../datas/ClientData.h"
#include "../SceneManager.h"
#endif

BasicProps::BasicProps()
: m_Owner(NULL)
{
    
}

BasicProps::~BasicProps()
{
    m_Owner = NULL;
}

bool BasicProps::init(ballMap* owner, int id, int classId)
{
    if (! CCSprite::init()) {
        return false;
    }

    m_Owner = owner;
    m_ID = id;
    m_ClassID = classId;
    
    return true;
}

BasicProps* BasicProps::create(ballMap* owner, int id, int classId)
{
    BasicProps* pRet = new BasicProps();
    if (! pRet->init(owner, id, classId)) {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
    
//    pRet->autorelease();
    return pRet;
}


int BasicProps::script_maskMap(lua_State* L)
{
	int result = 0;
	
	int paramCount = lua_gettop(L) - 2;
	if (paramCount < 0) {
        lua_pushboolean(L, result);
        return result;
    }
    
    int maskType = lua_tonumber(L, 3);
    getOwner()->maskMap(getPos(), maskType);
    
    result = 1;
    lua_pushboolean(L, result);
	return result;
    
}

int BasicProps::script_dismaskMap(lua_State* L)
{
	int result = 1;
	
    getOwner()->disMaskMap(getPos());
    
    lua_pushboolean(L, result);
    
    return result;
}

int BasicProps::callFunction(lua_State* L)
{
	const char* funcName = lua_tostring(L, 2);
	if (strcmp(funcName, "maskMap") == 0)
		return script_maskMap(L);
    else if (strcmp(funcName, "disMaskMap") == 0)
        return script_dismaskMap(L);
    return 1;
}


int BasicProps::show(int pos)
{
    const propsCfg* pPropsCfg = g_clientData->getPropsCfg(m_ClassID);
    if (! pPropsCfg || ! pPropsCfg->OnShow) return 0;

    EventMap::iterator it = pPropsCfg->pEventList->mEventFuncs.find("OnShow");
    if (it == pPropsCfg->pEventList->mEventFuncs.end()) return 0;

    m_pos = pos;
    SceneData* pSceneData = g_sceneManager->getCurScene();
    return g_gameScript->excuteFunction(it->second.key.c_str(), this, pSceneData, getOwner());
}

bool BasicProps::isAround(int pos)
{
    if (m_pos == pos) return true;
    
    int w = getOwner()->getWidth();
    int propsx = m_pos % w;
    int propsy = m_pos / w;
    int px = pos % w;
    int py = pos / w;
    
    for (int i = 0; i < 4; i++) {
        if (px + PATHDIRS[i].x == propsx && py + PATHDIRS[i].y == propsy) {
            return true;
        }
    }
    
    return false;
   
}

bool BasicProps::onRemoveBall(int pos)
{
    const propsCfg* pPropsCfg = g_clientData->getPropsCfg(m_ClassID);
    if (! pPropsCfg || ! pPropsCfg->OnAroundRemove) return false;
    
    EventMap::iterator it = pPropsCfg->pEventList->mEventFuncs.find("OnAroundRemove");
    if (it == pPropsCfg->pEventList->mEventFuncs.end()) return false;
    
    if (! isAround(pos)) return false;
    SceneData* pSceneData = g_sceneManager->getCurScene();
    return g_gameScript->excuteFunction(it->second.key.c_str(), this, pSceneData, getOwner());
}