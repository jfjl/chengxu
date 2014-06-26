#include "HelloWorldScene.h"
#include "ClientData.h"
#include "LevelDialog.h"
#include "BasicProps.h"

extern "C" {
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
#include "tolua_fix.h"
    
    //’‚¿ÔæÕ «πÿ¡™metatableµƒ__index”Ú£¨Œ“√«À˘”–µƒ≤Ÿ◊˜∂ºª·◊™ªªµΩ∫Ø ˝class_index_eventΩ¯––
    extern int class_index_event(lua_State *ls);
}

USING_NS_CC;

CCScene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    CCScene *scene = CCScene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !CCLayer::init() )
    {
        return false;
    }
    
    
    srand((unsigned)time(NULL));
    
    m_pGameScript = new GameScript();
    
    new ClientData();
    
    //scene
    m_pSceneManager = new SceneManager(this);
    m_pSceneManager->init();
    this->addChild(m_pSceneManager);
    
    //dialog
    m_pDialogManager = DialogManager::create(this);
    this->addChild(m_pDialogManager);
    //start
    m_pDialogManager->showDialog("LevelDialog");
    
    this->schedule(schedule_selector(HelloWorld::onUpdate), 0.1f);
    
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterGame), EVENT_ENTER_GAME, NULL);
    

    setScale(GAMESCALE);
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));

    return true;
}

void HelloWorld::onUpdate(float dt)
{
    m_pSceneManager->onUpdate(dt);
}

void HelloWorld::onEnterGame(CCObject* pEvent)
{
    DialogEvent* pDialogEvent = (DialogEvent*) pEvent;
    m_pDialogManager->hideDialog("LevelDialog");
    m_pDialogManager->showDialog("ScoreXDialog");
    m_pDialogManager->showDialog("PropsDialog");
    m_pSceneManager->setCurScene("gameScene", pDialogEvent);
//    m_pGameScript->loadInitScript();
}

