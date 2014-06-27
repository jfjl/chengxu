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
    , m_stepAffected(0)
{
	
}

gameScene::~gameScene(void)
{
    CC_SAFE_DELETE(m_BallMap);
    CC_SAFE_DELETE(m_TouchMap);
}


gameScene * gameScene::scene()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("scene.plist");
    //CCScene * scene = CCScene::create();
    
    gameScene* pRet = new gameScene;
    ballMap *pBallMap   = ballMap::create("next.png", NEXTMAPSIZE_WIDTH, NEXTMAPSIZE_HEIGHT);
    touchMap *pTouchMap = touchMap::create("background.png", GAMEMAPSIZE_WIDTH, GAMEMAPSIZE_HEIGHT);
    
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
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onSceneNext), EVENT_SCENE_NEXT, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onGameOver), EVENT_GAME_COMPLETE, NULL);
    pTouchMap->setTouchEnabled(true);

	srand(time(0));
    
    return true;
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
    this->setPosition(ccp(SCENEMARGIN_LEFT, SCENEMARGIN_TOP));
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
    if (m_vStepPosition.size() > 0) {
        for (int i = 0; i < m_vStepPosition.size(); i++) {
            pos->push_back(m_vStepPosition[i]);
            m_TouchMap->disMaskMap(m_vStepPosition[i]);
        }
    }else{
        ballMap->getRandomPosition(pos, count);
    }
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
    if (m_stepAffected > 0 && ! m_bStepEnabled) {
        decStepAffected();
        return;
    };
    
	ballVector *balls = m_BallMap->getBallManager()->getBallList();
	randomShowBall(balls);
    
    decStepAffected();
}

int gameScene::decStepAffected()
{
    if (m_stepAffected <= 0) return 0;
    
    int result = m_stepAffected--;
    if (m_stepAffected <= 0) {
        m_bStepEnabled = true;
        m_vStepPosition.clear();
        m_TouchMap->setTouchEnabled(true);
    }else{
        if (m_vStepPosition.size() > 0) {
            m_vStepPosition.clear();
            getRandomPosition(m_TouchMap, &m_vStepPosition);
        }
    }
    
    return result;
}


int gameScene::script_disableBallMap(lua_State* L)
{
	int result = 0;
	
	int paramCount = lua_gettop(L) - 2;
	if (paramCount < 0) {
        lua_pushboolean(L, result);
        return result;
    }
    
    m_stepAffected = lua_tonumber(L, 3);
    m_bStepEnabled = false;
    
    result = 1;
    return result;
}

int gameScene::script_showNextPosition(lua_State* L)
{
	int result = 0;
	
	int paramCount = lua_gettop(L) - 2;
	if (paramCount < 0) {
        lua_pushboolean(L, result);
        return result;
    }
    
    m_stepAffected = lua_tonumber(L, 3);
    m_vStepPosition.clear();
	getRandomPosition(m_TouchMap, &m_vStepPosition);

    for (int i = 0; i < m_vStepPosition.size(); i++ ) {
        m_TouchMap->maskMap(m_vStepPosition[i], MC_STATE_FLAG);
    }
    result = 1;
    
    return result;
}


int gameScene::callFunction(lua_State* L)
{
	const char* funcName = lua_tostring(L, 2);
	if (strcmp(funcName, "disableBallMap") == 0)
		return script_disableBallMap(L);
    else if (strcmp(funcName, "showNextPosition") == 0)
        return script_showNextPosition(L);
    
    return 1;
}


