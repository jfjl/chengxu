#include "HelloWorldScene.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ClientData.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "datas/ClientData.h"
#endif

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
    
    m_nLayerCount = 0;
    //scene
    m_pSceneManager = new SceneManager(this);
    m_pSceneManager->init();
    this->addChild(m_pSceneManager);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterGame),
                                                                  EVENT_ENTER_SCENELAYER, NULL);
    
    //dialog
    m_pDialogManager = DialogManager::create(this);
    this->addChild(m_pDialogManager);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterGame),
                                                                  EVENT_ENTER_DIALOGLAYER, NULL);
    
    //present
    m_pPresentManager = PresentManager::create(this);
    this->addChild(m_pPresentManager);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterGame),
                                                                  EVENT_ENTER_PRESENTLAYER, NULL);

    setScale(GAMESCALE);
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));
    
    return true;
}

void HelloWorld::update(float dt)
{
    m_pSceneManager->onUpdate(dt);
    m_pDialogManager->onUpdate(dt);
}

void HelloWorld::onEnterGame(CCObject* pEvent)
{
    if (++m_nLayerCount == 3){
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_ENTER_SCENELAYER);
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_ENTER_DIALOGLAYER);
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_ENTER_PRESENTLAYER);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onExitGame),
                                                                      EVENT_EXIT_SCENELAYER, NULL);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onExitGame),
                                                                      EVENT_EXIT_DIALOGLAYER, NULL);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onExitGame),
                                                                      EVENT_EXIT_PRESENTLAYER, NULL);
        
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterSetLevel),
                                                                      EVENT_ENTER_SETLEVEL, NULL);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterMainMenu),
                                                                      EVENT_ENTER_MAINMENU, NULL);
        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onStartLevel),
                                                                      EVENT_START_LEVEL, NULL);
        
        //start
        m_pDialogManager->showDialog("LevelDialog");
        //this->schedule(schedule_selector(HelloWorld::onUpdate), 0.001f);
        scheduleUpdate();

        CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterLevel), EVENT_ENTER_LEVEL, NULL);
    }
}

void HelloWorld::onExitGame(CCObject* pEvent)
{
    if (--m_nLayerCount == 0) {
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_EXIT_SCENELAYER);
        CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_EXIT_DIALOGLAYER);
    }
}

void HelloWorld::onEnterLevel(CCObject* pEvent)
{
    CCNotificationCenter::sharedNotificationCenter()->removeObserver(this, EVENT_ENTER_LEVEL);

    DialogEvent* pDialogEvent = (DialogEvent*) pEvent;
    m_pDialogManager->hideDialog("LevelDialog");
    m_pDialogManager->showDialog("ScoreDialog", pDialogEvent);
    m_pDialogManager->showDialog("PropsDialog");
    m_pSceneManager->setCurScene("gameScene", pDialogEvent);
}

void HelloWorld::onStartLevel(CCObject* pEvent)
{
    DialogEvent* pDialogEvent = (DialogEvent*) pEvent;
    SceneData* pSceneData = m_pSceneManager->getCurScene();
    if (pSceneData) {
        pSceneData->start(pDialogEvent);
    }
}

void HelloWorld::onEnterSetLevel(CCObject* pEvent)
{
    DialogEvent* pDialogEvent = (DialogEvent*) pEvent;

    m_pDialogManager->hideDialog("LevelDialog");
    m_pDialogManager->hideDialog("BattleResultDialog");
    m_pDialogManager->showDialog("ScoreDialog", pDialogEvent);
    m_pDialogManager->showDialog("PropsDialog");
    
}

void HelloWorld::onEnterMainMenu(CCObject* pEvent)
{
    m_pDialogManager->hideDialog("BattleResultDialog");
    m_pDialogManager->hideDialog("scoreDialog");
    m_pDialogManager->hideDialog("PropsDialog");
    m_pDialogManager->showDialog("LevelDialog");
}
