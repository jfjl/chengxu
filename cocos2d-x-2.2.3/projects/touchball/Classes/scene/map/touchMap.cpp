
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
	for (int i = 0; i < balls->count(); i++){
		ball *temp = (ball*) balls->objectAtIndex(i);
		this->getBallManager()->playHide(temp);
		this->playAction(temp);
	}
}

bool touchMap::checkAwardBall(CCArray* balls)
{
    /*m_actionBalls
	for (int i = 0; i < balls->count(); i++){
        
		ball *temp = (ball*) balls->objectAtIndex(i);
		this->getBallManager()->playHide(temp);
		this->playAction(temp);
	}
     */
    return true;
}

bool touchMap::checkRemove()
{
    int removeCount = REMOVECOUNT;
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(m_level);
	if (pLevelCfg)
        removeCount = pLevelCfg->RemoveCount;
    
    CCLog("set remove count %d", removeCount);
    CCArray *balls = new CCArray();

	CCDictElement *pElement;
	CCDICT_FOREACH(m_actionBalls, pElement) {
		ball *pball = (ball*) pElement->getObject();
		CCPoint pos = pball->getPos();
		CCArray *removeBalls = m_BallManager->checkRemove(pos.x, pos.y, removeCount);
        
        CCLog("RemoveData %d", removeBalls->count());
		if (removeBalls->count() >= removeCount - 1){
			balls->addObjectsFromArray(removeBalls);
			balls->addObject(pball);
            CCLog("add remove ball %d", pball->getClassID());
		}
		removeBalls->removeAllObjects();
		CC_SAFE_DELETE(removeBalls);
	}

	bool result = balls->count() >= removeCount;
	if (result){
        checkAwardBall(balls);
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

	if (checkOver())
		CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_GAME_COMPLETE, (CCObject*)this);
	else{
		checkRemove();
	}
	
	m_actionBalls->removeAllObjects();
}

void touchMap::onHideComplete(CCObject *pball)
{
	if (! onEnterEvent(pball)) return;

	CCNotificationCenter::sharedNotificationCenter()->postNotification(EVENT_SCENE_NEXT, (CCObject*)this);
	m_actionBalls->removeAllObjects();
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
	++m_ActionCount;
	m_actionBalls->setObject(pball, pball->getID());
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
        int curp = getCurMovePos();
        CCLog("cur pos %d", curp);
        ball *pball = m_BallManager->getBall(getCurMovePos());
        CCLog("class %d", pball->getClassID());
        CCLog("next pos %d", nextPos);
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
	float s = MAPCELL_SIZE * TEXTURESCALE;
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
			//move
            setCurMovePos(oldSel);
            setMapState(MapStateMove);
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
