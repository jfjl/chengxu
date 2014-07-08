//
//  gameScene.cpp
//  ppball
//
//  Created by carlor on 13-3-14.
//
//

#include "gameScene.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "DialogEvent.h"
#include "ClientData.h"
#include "GameScript.h"
#include "UserLocalData.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../ui/DialogEvent.h"
#include "../datas/ClientData.h"
#include "../script/GameScript.h"
#include "../utils/UserLocalData.h"
#endif

gameScene::gameScene()
    : m_Level(0)
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
    touchMap *pTouchMap = touchMap::create("map_01.png", GAMEMAPSIZE_WIDTH, GAMEMAPSIZE_HEIGHT);
    
    pRet->init(pBallMap, pTouchMap);
    pRet->autorelease();
    
    //scene->addChild(pRet);
    
    return pRet;
}

bool gameScene::init(ballMap* pBallMap, touchMap* pTouchMap)
{
    m_nRefreshTime = 0.01;
    
	CCSprite* back = CCSprite::createWithSpriteFrameName("background.png");
    back->setAnchorPoint(ccp(0, 0));
    this->addChild(back);
    
	//map
    setTouchMap(pTouchMap);
    addChild(pTouchMap, 1);

    setBallMap(pBallMap);
    pBallMap->setPosition(ccp(350, 830));
    addChild(pBallMap, 2);
    
    this->setAnchorPoint(ccp(0, 0));
    this->setPosition(ccp(0, 0));

    pBallMap->createBalls();
    pTouchMap->createBalls();

	//event
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onSceneNext), EVENT_SCENE_NEXT, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onGameOver), EVENT_GAME_COMPLETE, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(gameScene::onBallRemove), EVENT_SCENE_REMOVE, NULL);
    
    pTouchMap->setTouchEnabled(true);

	srand(time(0));
    
    return true;
}

bool gameScene::checkOverStep()
{
    return --m_Step <= 0;
}

void gameScene::onSceneNext(CCObject *ptouchMap)
{
    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->setkey("step");
    pDialogEvent->setvalue(m_Step-1);
    pDialogEvent->autorelease();
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_STEP_CHANGE, pDialogEvent);
    
    if (checkOverStep()) {
        onGameOver(NULL);
        return;
    }
	next();
}

void gameScene::onBallRemove(CCObject* balls)
{
    m_Score += ((CCArray*) balls)->count();
    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->setkey("count");
    pDialogEvent->setvalue(((CCArray*) balls)->count());
    pDialogEvent->autorelease();
    
    CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SCORE_CHANGE, pDialogEvent);
}

void gameScene::onGameOver(CCObject *ptouchMap)
{
    int star = 0;
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_Level);
    if (pLevelCfg) {
        if (m_Score >= pLevelCfg->Score3)
            star = 3;
        else if (m_Score >= pLevelCfg->Score2)
            star = 2;
        else if (m_Score >= pLevelCfg->Score1)
            star = 1;
    }
    UserLocalData::setLevelInfo(getLevel(), m_Score, star);
    
    DialogEvent* pDialogEvent = new DialogEvent();
    pDialogEvent->addKeyValue("result", star > 0 ? 1 : 0);
    pDialogEvent->addKeyValue("score", m_Score);
    pDialogEvent->addKeyValue("star", star);
    pDialogEvent->autorelease();
    g_dialogManager->showDialog("BattleResultDialog", pDialogEvent);
}

bool gameScene::onUpdate(float dt)
{
    bool result = SceneData::onUpdate(dt);
        
    if (result) {
        m_TouchMap->onUpate(dt);
    }
    
    return result;
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

void gameScene::start(void* param)
{
    m_Score = 0;
    DialogEvent* pDialogEvent = (DialogEvent* ) param;
    m_Level = pDialogEvent->getvalue();
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_Level);
    if (pLevelCfg) {
        m_Step = pLevelCfg->MaxStep;
    }else{
        m_Step = 10;
    }
    
    m_TouchMap->setLevel(m_Level);
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


