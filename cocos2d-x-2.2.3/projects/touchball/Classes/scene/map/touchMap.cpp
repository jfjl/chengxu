
//  touchMap.cpp
//  ppball
//
//  Created by carlor on 13-3-13.
//
//

#include "touchMap.h"
#include "ClientData.h"

touchMap::touchMap(void)
    : m_PathFinder(NULL),
	m_actionBalls(NULL)
{
    
}


touchMap::~touchMap(void)
{
	CC_SAFE_DELETE(m_actionBalls);
    CC_SAFE_DELETE(m_PathFinder);
    m_mShapes.clear();
}


bool touchMap::init(const char*fileName, int width, int height)
{
	if (ballMap::init(fileName, width, height)){
		setTouchEnabled(true);
        setMapState(MapStateNormal);
        m_PathFinder = pathFinder::create(this, getWidth(), getHeight());
        m_actionBalls = CCDictionary::create();
		m_actionBalls->retain();

		return true;
	}
    
	return false;
}

touchMap* touchMap::create(const char*fileName, int width, int height)
{
    touchMap *pTouchMap = new touchMap();
    pTouchMap->init(fileName, width, height);
    pTouchMap->autorelease();
    return pTouchMap;
}
//////////////event
bool touchMap::checkOver()
{
	CCArray *visballs = new CCArray();
	this->getBallManager()->getVisibleBall(false, visballs);
	bool isOver = visballs->count() <= 0;
	visballs->removeAllObjects();
	
	CC_SAFE_DELETE(visballs);
	return isOver;
}

void touchMap::remove(CCArray *balls)
{
    setActionCount(balls->count());
	for (int i = 0; i < balls->count(); i++){
		ball *temp = (ball*) balls->objectAtIndex(i);
		this->playAction(temp);
		this->getBallManager()->playHide(temp);
	}
}

bool touchMap::checkRemove()
{
    int removeCount = REMOVECOUNT;
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_level);
	if (pLevelCfg)
        removeCount = pLevelCfg->RemoveCount;
    
    
    m_mShapes.clear();
    
    CCArray *balls = new CCArray();
	CCDictElement *pElement;
	CCDICT_FOREACH(m_actionBalls, pElement) {
		ball *pball = (ball*) pElement->getObject();
        int shape = 0;
		CCArray *removeBalls = m_BallManager->checkRemove(pball, removeCount, shape);
        
		if (removeBalls->count() >= removeCount - 1){
            for (int i = 0; i < removeBalls->count(); i++) {
                ball* tempball = (ball*) removeBalls->objectAtIndex(i);
                if (balls->containsObject(tempball)) continue;
                balls->addObject(tempball);
            }
            if (! balls->containsObject(pball)) {
                balls->addObject(pball);
                if (shape)
                    m_mShapes[m_BallManager->getKey(pball->getPos().x, pball->getPos().y)] = shape;
            }
		}
		removeBalls->removeAllObjects();
		CC_SAFE_DELETE(removeBalls);
	}

	bool result = balls->count() >= removeCount;
	if (result){
        m_BallManager->getRemoveBall(balls);
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SCENE_REMOVE, (CCObject*)balls);
        remove(balls);
	}
	balls->removeAllObjects();
	CC_SAFE_DELETE(balls);

	return result;
}

bool touchMap::onEnterEvent(CCObject *pball)
{
	//ball *temp = dynamic_cast<ball *> (pball);
	//m_actionBalls->removeObjectForKey(temp->getID());
	return --m_ActionCount <= 0;
}

void touchMap::onShowComplete(CCObject *pball)
{
	if (! onEnterEvent(pball)) return;

    if (! checkRemove())
    {
        if (checkOver())
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_GAME_COMPLETE, (CCObject*)this);
    }
	
	m_actionBalls->removeAllObjects();
}

void touchMap::onHideComplete(CCObject *pball)
{
	if (! onEnterEvent(pball)) return;

	m_actionBalls->removeAllObjects();
    
    //检查是否出现奖励球
    bool bShow = false;
    for (map<int, int>::iterator it = m_mShapes.begin(); it != m_mShapes.end(); it++) {
        int px = it->first % GAMEMAPSIZE_WIDTH;
        int py = it->first / GAMEMAPSIZE_WIDTH;
        ball* pball = m_BallManager->getBall(px, py);
        if (! pball) continue;
        const ballCfg* pballCfg = g_clientData->getBallCfg(pball->getClassID());
        if (! pballCfg) continue;
        
        int shape = it->second;
        const awardBallCfg* pawardBallCfg = g_clientData->getAwardBallCfg(shape);
        if (! pawardBallCfg) continue;
        
        int id = pballCfg->nBasicBall + pawardBallCfg->BallType * 10;
        const ballCfg* pawardCfg = g_clientData->getBallCfg(id);
        if (! pawardCfg) continue;
        
        incActionCount();
		playAction(pball);
        m_BallManager->playShow(id, px, py);
        bShow = true;
    }
    
    if (! bShow)
        CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SCENE_NEXT, (CCObject*)this);
}

void touchMap::onMoveComplete(ball *pball)
{
	m_ActionCount = 0;
	m_actionBalls->removeAllObjects();
	m_actionBalls->setObject(pball, pball->getID());

	if (! checkRemove())
		CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SCENE_NEXT, (CCObject*)this);
}

void touchMap::addEventLister(CCObject *pball)
{
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(touchMap::onShowComplete),
		EVENT_ACTION_SHOWCOMPLETE, NULL);
	CCNotificationCenter::sharedNotificationCenter()->addObserver(this, callfuncO_selector(touchMap::onHideComplete), 
		EVENT_ACTION_HIDECOMPLETE, NULL);
}

void touchMap::playAction(ball *pball)
{
 	m_actionBalls->setObject(pball, pball->getID());
}

void touchMap::incActionCount()
{
    m_ActionCount++;
}

///////////////////////update
void touchMap::onUpate(float dt)
{
    bool nextStep = true;
    
    switch (getMapState()) {
        case MapStateShow:
            break;
        case MapStateMove:
            nextStep = onUpdateMove(dt);
            break;
        case MapStateHide:
            break;
        default:
            break;
    }
    
    if (nextStep){
        setMapState(MapStateNormal);
	}
}

bool touchMap::onUpdateMove(float dt)
{
    int nextPos = m_PathFinder->getNextNode();
    if (nextPos >= 0){
        ball *pball = m_BallManager->getBall(getCurMovePos());
        m_BallManager->hide(pball->getID());
        setCurMovePos(nextPos);
        m_BallManager->show(pball->getClassID(), nextPos);
    }
	if (nextPos < 0){
		ball *pball = m_BallManager->getBall(m_PathFinder->getDestPosition());
		onMoveComplete(pball);
	}
    return nextPos < 0;
}


/////////////////////////event
void touchMap::registerWithTouchDispatcher(void)
{
    CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
}

CCPoint touchMap::getLocalPoint(CCPoint value)
{
	float s = MAPCELL_SIZE * TEXTURESCALE * GAMESCALE;
	return CCPointMake(floor(value.x / s), floor(value.y / s));
}

bool touchMap::checkMove(int srcPos, int x, int y)
{
    if (m_PathFinder->inBlackList(y*getWidth()+x)) return false;
    
    return m_PathFinder->onMoveBall(srcPos, y*getWidth()+x);
}

bool touchMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
    
    int oldSel = getBallManager()->getSelectId();
	CCPoint p = getLocalPoint(touchPoint);
	int id = getBallManager()->select(p.x, p.y);
	
	if (id < 0 && oldSel >= 0){
		if (checkMove(oldSel, p.x, p.y)){
            CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_BALL_MOVE, (CCObject *)this);
            
			//move
            setCurMovePos(oldSel);
            setMapState(MapStateMove);
            getBallManager()->setSelectId(-1);
		}
	}
    
    return false;
}

void touchMap::ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent)
{
    
}

void touchMap::ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
}

void touchMap::ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent)
{
    
}
////////
void touchMap::setLevel(int level)
{
    if (m_level == level) return;
    
    m_level = level;
    
    m_PathFinder->initBlackList(level);

    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(level);
    if (! pLevelCfg)
    {
        return;
        //CCSpriteFrame *frame=CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName("headleft.png");
        //m_Backgound->setDisplayFrame(frame);
    }
    const mapCfg* pMapCfg = g_clientData->getMapCfg(pLevelCfg->MapId);
    if (! pMapCfg)
    {
        return;
    }
    
    
    float s = MAPCELL_SIZE * TEXTURESCALE;
    int w = getWidth();

    for (size_t i = 0; i < pMapCfg->vMapCell.size(); i++)
    {
        if (pMapCfg->vMapCell[i] == 0) continue;
        CCSprite* pSprite = CCSprite::createWithSpriteFrameName("mask.png");
        pSprite->setColor(ccc3(255, 0, 0));
        pSprite->setContentSize(CCSize(s, s));

        int x = i % w;
        int y = i / w;
        pSprite->setPosition(ccp(s * x + s / 2, s * y + s / 2));
        
        this->addChild(pSprite);
    }
}

int touchMap::getLevel()
{
    return m_level;
}

void touchMap::getRandomPosition(std::vector<int> *pos, int count)
{
    CCArray *visballs = new CCArray();
    m_BallManager->getVisibleBall(false, visballs);
    
    while (pos->size() < count && visballs->count() > 0){
        int index = rand() % visballs->count();
        int p = dynamic_cast<ball*>(visballs->objectAtIndex(index))->getID();
        if (! m_PathFinder->inBlackList(p))
            pos->push_back(p);
        visballs->removeObjectAtIndex(index);
    }
	CC_SAFE_DELETE(visballs);
}
