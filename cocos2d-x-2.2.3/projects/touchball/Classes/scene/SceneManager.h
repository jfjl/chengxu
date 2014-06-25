//
//  SceneManager.h
//  touchball
//
//  Created by carlor on 14-6-6.
//
//

#ifndef __touchball__SceneManager__
#define __touchball__SceneManager__

#include "SceneData.h"

class SceneManager
{
public:
    SceneManager(CCNode* pOwner);
    ~SceneManager();
    
private:
    void clear();
    
public:
    bool init();
    void onUpdate(float dt);

    void setCurScene(const char* sSceneName, void* param = NULL);
    SceneData* getCurScene();
private:
    CCNode* m_pOwner;
    std::map<const char*, SceneData*> m_mSceneList;
    SceneData* m_pActivateScene;
};

extern	SceneManager* g_sceneManager;

#endif /* defined(__touchball__SceneManager__) */
