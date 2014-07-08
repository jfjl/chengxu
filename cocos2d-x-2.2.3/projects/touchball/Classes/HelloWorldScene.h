#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "SceneManager.h"
#include "DialogManager.h"
#include "PresentManager.h"
#include "GameScript.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "scene/SceneManager.h"
#include "ui/DialogManager.h"
#include "present/PresentManager.h"
#include "script/GameScript.h"
#endif

class HelloWorld : public cocos2d::CCLayer
{
public:
    // Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
    virtual bool init();  

    // there's no 'id' in cpp, so we recommend returning the class instance pointer
    static cocos2d::CCScene* scene();
    
    // implement the "static node()" method manually
    CREATE_FUNC(HelloWorld);
private:
    void update(float dt);
    void onEnterGame(CCObject* pEvent);
    void onExitGame(CCObject* pEvent);
    void onEnterLevel(CCObject* pEvent);
    void onStartLevel(CCObject* pEvent);
    
    void onEnterSetLevel(CCObject* pEvent);
    void onEnterMainMenu(CCObject* pEvent);
private:
    int m_nLayerCount;
    SceneManager* m_pSceneManager;
    DialogManager* m_pDialogManager;
    PresentManager* m_pPresentManager;
    GameScript* m_pGameScript;
};

#endif // __HELLOWORLD_SCENE_H__
