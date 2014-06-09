//
//  SceneManager.cpp
//  touchball
//
//  Created by carlor on 14-6-6.
//
//

#include "SceneManager.h"
#include "gameScene.h"

SceneManager::SceneManager(CCNode* pOwner)
:m_pActivateScene(NULL),
 m_pOwner(pOwner)
{
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

void SceneManager::onUpdate(float dt)
{
    if (m_pActivateScene) {
        m_pActivateScene->onUpdate(dt);
    }
}

void SceneManager::setCurScene(const char* sSceneName)
{
    if (m_pActivateScene)
        m_pActivateScene->onDeactivate();
    
    if (strncmp(sSceneName, "gameScene", strlen(sSceneName)) == 0)
    {
        gameScene* gameScene = gameScene::scene();
        gameScene->onActivate(m_pOwner);
        
        m_pActivateScene = gameScene;
    }
}
