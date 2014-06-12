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

void gameScene::onActivate(CCNode* pNode)
{
    SceneData::onActivate(pNode);
    start(1);
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
        rate = rand() % 100;
        for (size_t i = 0; i < pLevelCfg->vSpecialBallIds.size(); i++)
        {
            if (rate > pLevelCfg->vSpecialBallIds[i].value) continue;
            return pLevelCfg->vSpecialBallIds[i].key;
        }
        
        int index = rand() % pLevelCfg->vBallIds.size();
        return pLevelCfg->vBallIds[index];
    }
}

void gameScene::getRandomPosition(touchMap *ballMap, std::vector<int> *pos, int count)
{
    ballMap->getRandomPosition(pos, count);
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
}

void gameScene::randomShowBall(ballVector *balls)
{
	std::vector<int> *pos = new std::vector<int>();
	getRandomPosition(m_TouchMap, pos);
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
	randomShowBall(balls);
}




