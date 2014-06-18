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
    
    return true;
}

void SceneData::onUpdate(float dt)
{
    
}


void SceneData::onActivate(CCNode* pNode, void* param)
{
    pNode->addChild(this, 100);
}

void SceneData::onDeactivate()
{
    this->removeFromParentAndCleanup(true);
}