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
    
//    
//    lua_State *L = lua_open();
//    luaopen_base(L);
//    
//    lua_pushcfunction(L, CreateCTest);    // 注册用于创建类的全局函数
//    lua_setglobal(L,  "CTest");
//    
//    luaL_newmetatable(L, "CTest");           // 创建一个元表
//    
//    lua_pushstring(L, "__gc");               // 垃圾回收
//    lua_pushcfunction(L, DestoryCTest);
//    lua_settable(L, -3);                     // 公共函数调用的实现就在此啊
//    
//    lua_pushstring(L, "__index");
//    lua_pushcfunction(L, lua_index);
//    lua_settable(L, -3);
//    
//    lua_pushstring(L, "__newindex");
//    lua_pushcfunction(L, lua_newindex);
//    lua_settable(L, -3);
//    std::string path = cocos2d::CCFileUtils::sharedFileUtils()->fullPathForFilename("init.lua");
//    luaL_dofile(L, path.c_str());
//    lua_pop(L,1);
//    
//    const char* code = "function test(p) print('test');p.Add(1, 2);end";
//    int result = luaL_loadbuffer(L, code, strlen(code), "line");
//    
//	if (result == 0){
//		result = lua_pcall(L, 0, LUA_MULTRET, 0);
//		lua_pushstring(L, code);
//	}
//    
//    
//    CTest* p = new CTest();
//    CCLOG("%D", p->m_x);
//    // 往lua中注册类
//    lua_getglobal(L, "test");
//    int size = sizeof(void* );
//    void* pData = lua_newuserdata(L, size);
//    pData = p;
//	lua_getfield(L, LUA_REGISTRYINDEX, "CTest");
//	lua_setmetatable(L, -2);
//    
//    lua_pcall(L, 1, 0, NULL);
//    
//    
//    
//    lua_close(L);
    
    srand((unsigned)time(NULL));
    
    m_pGameScript = new GameScript();
    
    new ClientData();
    
    //scene
    m_pSceneManager = new SceneManager(this);
    m_pSceneManager->init();
    
    //dialog
    m_pDialogManager = DialogManager::create(this);
    //start
    m_pDialogManager->showDialog("LevelDialog", this);
    
    this->schedule(schedule_selector(HelloWorld::onUpdate), 0.1f);
    
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(HelloWorld::onEnterGame), EVENT_ENTER_GAME, NULL);
    

    setScale(2);
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
    m_pDialogManager->showDialog("ScoreXDialog", this);
    m_pDialogManager->showDialog("PropsDialog", this);
    m_pSceneManager->setCurScene("gameScene", pDialogEvent);
//    m_pGameScript->loadInitScript();
}

