#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "SceneManager.h"
#include "DialogManager.h"
#include "GameScript.h"

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
    void onUpdate(float dt);
    void onEnterGame(CCObject* pEvent);
    
private:
    SceneManager* m_pSceneManager;
    DialogManager* m_pDialogManager;
    GameScript* m_pGameScript;
};

#endif // __HELLOWORLD_SCENE_H__
