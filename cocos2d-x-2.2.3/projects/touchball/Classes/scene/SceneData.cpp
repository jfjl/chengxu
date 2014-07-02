//
//  SceneData.cpp
//  touchball
//
//  Created by carlor on 14-6-6.
//
//

#include "SceneData.h"

SceneData * SceneData::scene()
{
    CCScene * scene = CCScene::create();
    
    SceneData* layer = SceneData::create();
    
    scene->addChild(layer);
    
    return layer;
}


bool SceneData::init()
{
    if (! CCLayer::init())
    {
        return false;
    }
    
    m_nRefreshTime = 0.1;
    m_nLastRefreshTime = 0;
    
    return true;
}

bool SceneData::onUpdate(float dt)
{
    if (m_nLastRefreshTime == 0 || m_nLastRefreshTime < 0.0001) return true;
    if (m_nLastRefreshTime - dt < m_nRefreshTime) return false;
    
    m_nLastRefreshTime = dt;
    return true;
}


void SceneData::onActivate(CCNode* pNode, void* param)
{
    pNode->addChild(this, 100);
}

void SceneData::start(void* param)
{
    
}

void SceneData::onDeactivate()
{
    this->removeFromParentAndCleanup(true);
}