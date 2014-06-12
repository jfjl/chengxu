#include "HelloWorldScene.h"
#include "ClientData.h"

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
    
    new ClientData();
    
    //scene
    m_pSceneManager = new SceneManager(this);
    m_pSceneManager->init();
    //dialog
    m_pDialogManager = DialogManager::create(this);
    //start
    m_pSceneManager->setCurScene("gameScene");
    m_pDialogManager->showDialog("ScoreDialog", this);
    
    this->schedule(schedule_selector(HelloWorld::onUpdate), 0.1f);

    setScale(2);
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));

    return true;
}

void HelloWorld::onUpdate(float dt)
{
    m_pSceneManager->onUpdate(dt);
}
