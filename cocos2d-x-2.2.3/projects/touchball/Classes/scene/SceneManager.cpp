//
//  SceneManager.cpp
//  touchball
//
//  Created by carlor on 14-6-6.
//
//

#include "SceneManager.h"
#include "gameScene.h"

SceneManager* g_sceneManager = 0;

SceneManager::SceneManager(CCNode* pOwner)
:m_pActivateScene(NULL),
 m_pOwner(pOwner)
{
    CCAssert(g_sceneManager == NULL, "重复创建 scenemanager");
    g_sceneManager = this;
}

SceneManager::~SceneManager()
{
    m_pActivateScene = NULL;
    clear();
}

void SceneManager::clear()
{
    m_mSceneList.clear();
}

bool SceneManager::init()
{
    clear();
    
    return true;
}

void SceneManager::onEnter()
{
    CCLayer::onEnter();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_ENTER_SCENELAYER, (CCObject*)this);
}

void SceneManager::onExit()
{
    CCLayer::onExit();
	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_EXIT_SCENELAYER, (CCObject*)this);
}

void SceneManager::onUpdate(float dt)
{
    if (m_pActivateScene) {
        m_pActivateScene->onUpdate(dt);
    }
}

void SceneManager::setCurScene(const char* sSceneName, void* param)
{
    if (m_pActivateScene)
        m_pActivateScene->onDeactivate();
    
    if (strncmp(sSceneName, "gameScene", strlen(sSceneName)) == 0)
    {
        gameScene* gameScene = gameScene::scene();
        gameScene->onActivate(this, param);
        
        m_pActivateScene = gameScene;
    }
}

SceneData* SceneManager::getCurScene()
{
    return m_pActivateScene;
}

