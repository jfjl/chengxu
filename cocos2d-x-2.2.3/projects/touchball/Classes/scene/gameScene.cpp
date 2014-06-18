//
//  gameScene.cpp
//  ppball
//
//  Created by carlor on 13-3-14.
//
//

#include "gameScene.h"
#include "DialogEvent.h"
#include "ClientData.h"
#include "GameScript.h"

gameScene::gameScene()
    : m_Level(0)
	, m_Score(NULL)
	, m_Top(NULL)
{
	
}

gameScene::~gameScene(void)
{
    CC_SAFE_DELETE(m_BallMap);
    CC_SAFE_DELETE(m_TouchMap);
}


gameScene * gameScene::scene()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("1.plist");
    //CCScene * scene = CCScene::create();
    
    gameScene* pRet = new gameScene;
    ballMap *pBallMap   = ballMap::create("next.png", NEXTMAPSIZE_WIDTH, NEXTMAPSIZE_HEIGHT);
    touchMap *pTouchMap = touchMap::create("back.png", GAMEMAPSIZE_WIDTH, GAMEMAPSIZE_HEIGHT);
    g_gameScript->registerObject("touchMap", pTouchMap);
    
    pRet->init(pBallMap, pTouchMap);
    pRet->autorelease();
    
    //scene->addChild(pRet);
    
    return pRet;
}

bool gameScene::init(ballMap* pBallMap, touchMap* pTouchMap)
{
	//map
    setTouchMap(pTouchMap);
    addChild(pTouchMap, 1);

    setBallMap(pBallMap);
    pBallMap->setPosition(ccp(pTouchMap->getMapSize().width + pBallMap->getMapSize().width / pBallMap->getWidth(),
                              pTouchMap->getMapSize().height - pBallMap->getMapSize().height));
    addChild(pBallMap, 2);
    
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));

    pBallMap->createBalls();
    pTouchMap->createBalls();

	//event
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onSceneRemove), EVENT_SCENE_REMOVE, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onSceneNext), EVENT_SCENE_NEXT, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onGameOver), EVENT_GAME_COMPLETE, NULL);
    pTouchMap->setTouchEnabled(true);

	srand(time(0));
    
    g_gameScript->registerObject("gameScene", this);
        
    return true;
}

void gameScene::onSceneRemove(CCObject *balls)
{
    DialogEvent* dialogEvent = new DialogEvent();
    dialogEvent->setkey("Score");
    
    int score = 100;
	CCArray *temp = (CCArray*) balls;
    
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_Level);
    int removeCount = REMOVECOUNT;
    if (pLevelCfg)
        removeCount = pLevelCfg->RemoveCount;
    
    if (temp->count() - removeCount > 0){
        for (int i = 0; i < temp->count() - removeCount; i++){
            score += i * 30;
        }
    }
    dialogEvent->setvalue(score);
    
    //CCNode* pnode = m_dialogManger->getDialog("ScoreDialog");
	//CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SCORE_CHANGE, (CCObject *)dialogEvent);
}

void gameScene::onSceneNext(CCObject *ptouchMap)
{
	next();
}

void gameScene::onGameOver(CCObject *ptouchMap)
{

}

void gameScene::onUpdate(float dt)
{
    SceneData::onUpdate(dt);
    m_TouchMap->onUpate(dt);
}

void gameScene::onActivate(CCNode* pNode, void* param)
{
    SceneData::onActivate(pNode, param);
    DialogEvent* pDialogEvent = (DialogEvent* ) param;
    start(pDialogEvent->getvalue());
}

void gameScene::onDeactivate()
{
    SceneData::onDeactivate();
}


int gameScene::getXByPosKey(int posKey, int width)
{
    return posKey % width;
}

int gameScene::getYByPosKey(int posKey, int width)
{
    return posKey / width;
}

int gameScene::getRandomType()
{
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_Level);
    if (! pLevelCfg)
    {
        CCLog("warn=====>get random ball failed, not level");
        return MAGICBALL_ID;
    }
    
    int rate = rand() % 100;
    if (rate <= pLevelCfg->MagicBallRate)
    {
        return MAGICBALL_ID;
    }
    else
    {
        rate = rand() % 1000;
        for (size_t i = 0; i < pLevelCfg->vSpecialBallIds.size(); i++)
        {
            if (rate > pLevelCfg->vSpecialBallIds[i].value) continue;
            int id = pLevelCfg->vSpecialBallIds[i].key;//(pLevelCfg->vSpecialBallIds[i].key / 1000) * 1000 + 1; //pLevelCfg->vSpecialBallIds[i].key;
            return id;
        }

        int index = rand() % pLevelCfg->vBallIds.size();
        return pLevelCfg->vBallIds[index];
    }
}

void gameScene::getRandomPosition(touchMap *ballMap, std::vector<int> *pos, int count)
{
    ballMap->getRandomPosition(pos, count);
}

void gameScene::randomShowMask(vector<int>* pos)
{
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_Level);
    if (! pLevelCfg) return;
    
    
}

void gameScene::randomShowBall()
{
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_Level);
    if (! pLevelCfg)
    {
        CCLog("error=====>init show ball failed, not level");
        return;
    }
    
    std::vector<int> *pos = new std::vector<int>();
    getRandomPosition(m_TouchMap, pos, pLevelCfg->InitCount);
    m_TouchMap->setActionCount(pos->size());
    for (int i = 0; i < pos->size(); i++) {
		ball *pball = m_TouchMap->getBallManager()->getBall(pos->at(i));
		m_TouchMap->playAction(pball);
        int classId = getRandomType();

        m_TouchMap->getBallManager()->playShow(classId,
                                           getXByPosKey(pos->at(i), m_TouchMap->getWidth()),
                                           getYByPosKey(pos->at(i), m_TouchMap->getWidth()));
    }

    for (int i = 0; i < NEXTCOUNT; i++){
        int classId = getRandomType();
        m_BallMap->getBallManager()->show(classId,
                                          getXByPosKey(i, m_BallMap->getWidth()),
                                          getYByPosKey(i, m_BallMap->getWidth()));
    }
    
    randomShowMask(pos);
}

void gameScene::randomShowBall(ballVector *balls)
{
	std::vector<int> *pos = new std::vector<int>();
	getRandomPosition(m_TouchMap, pos);
    m_TouchMap->setActionCount(pos->size());
	for (int i = 0; i < pos->size(); i++) {
		ball *srcball = balls->at(i);
		ball *pball = m_TouchMap->getBallManager()->getBall(pos->at(i));
		m_TouchMap->playAction(pball);
		m_TouchMap->getBallManager()->playShow(srcball->getClassID(),
			getXByPosKey(pos->at(i), m_TouchMap->getWidth()),
			getYByPosKey(pos->at(i), m_TouchMap->getWidth()));
	}
    
	for (int i = 0; i < NEXTCOUNT; i++){
        int classid = getRandomType();
		m_BallMap->getBallManager()->show(classid,
			getXByPosKey(i, m_BallMap->getWidth()),
			getYByPosKey(i, m_BallMap->getWidth()));
	}
}

void gameScene::start(int level)
{
    //m_dialogManger->showDialog("ScoreDialog");
    
    setLevel(level);
    m_TouchMap->setLevel(level);
    randomShowBall();
}

void gameScene::next()
{
	ballVector *balls = m_BallMap->getBallManager()->getBallList();
    CCLOG("%p", balls);
	randomShowBall(balls);
}
////////////////////////////////////

int gameScene::script_start(void* param)
{
	LuaScript* luaScript = (LuaScript*) param;
	int result = 0;
	
	int paramCount = luaScript->getStackSize() - 1;
	if (paramCount < 0) return result;
    
	int paramData[255];
	int msgId = luaScript->getInteger(1);
	for (int i = 0; i < paramCount; i++){
		paramData[i] = luaScript->getInteger(i+2);
	}
	//postMessage(msgId, paramData);
    
	return result;
    
}

void gameScene::scriptQuery(void* msg)
{
	ScriptMessage* sm = (ScriptMessage*) msg;
	if (sm->query == "start"){
//		DispatcherScriptMethod edsm = & gameScene::script_start;
//		sm->script->pushMethod(this, static_cast<ScriptMethod>(edsm));
	}
}



