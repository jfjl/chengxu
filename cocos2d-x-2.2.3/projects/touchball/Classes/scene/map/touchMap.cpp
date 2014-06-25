
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
	m_actionBalls(NULL),
    m_pPropsManager(NULL)
{
    
}


touchMap::~touchMap(void)
{
    clear();
	CC_SAFE_DELETE(m_actionBalls);
    CC_SAFE_DELETE(m_PathFinder);
    CC_SAFE_DELETE(m_mapCells);
    CC_SAFE_DELETE(m_pPropsManager);
    m_mShapes.clear();
}

void touchMap::clear()
{
    m_actionBalls->removeAllObjects();
    m_mMaskSprite.clear();
}

bool touchMap::init(const char*fileName, int width, int height)
{
	if (ballMap::init(fileName, width, height)){
		setTouchEnabled(true);
        setMapState(MapStateNormal);
        m_PathFinder = pathFinder::create(this, getWidth(), getHeight());
        m_actionBalls = CCDictionary::create();
		m_actionBalls->retain();

        m_pPropsManager = PropsManager::create(this, width, height);
        this->addChild(m_pPropsManager);
        m_mMaskSprite.clear();
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
	ball *temp = dynamic_cast<ball *> (pball);
    m_pPropsManager->onRemoveBall(getPosition(temp->getPos().x, temp->getPos().y));
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
    if (m_PathFinder->inBlockList(y*getWidth()+x)) return false;
    
    return m_PathFinder->onMoveBall(srcPos, y*getWidth()+x);
}

bool touchMap::ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent)
{
    CCPoint touchPoint = pTouch->getLocationInView();
    touchPoint = CCDirector::sharedDirector()->convertToGL( touchPoint );
    
    int oldSel = getBallManager()->getSelectId();
	CCPoint p = getLocalPoint(touchPoint);
    if (p.x < 0 || p.x >= m_Width || p.y < 0 || p.y >= m_Height) return false;

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
    
    m_pPropsManager->put(getPosition(p.x, p.y));
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

void touchMap::createMask(int pos, int r, int g, int b)
{
    float s = MAPCELL_SIZE * TEXTURESCALE;
    int w = getWidth();
    

    CCLayerColor* pSprite = CCLayerColor::create();//("mask.png");
    pSprite->setColor(ccc3(r, g, b));
    pSprite->setContentSize(CCSize(s, s));
    pSprite->setOpacity(150);
    
    int x = pos % w;
    int y = pos / w;
    pSprite->setPosition(ccp(s * x, s * y));
    
    this->addChild(pSprite);
    m_mMaskSprite[pos] = pSprite;
    
}

void touchMap::deleteMask(int pos)
{
    map<int, CCLayerColor*>::iterator it = m_mMaskSprite.find(pos);
    if (it == m_mMaskSprite.end()) return;
    
    this->removeChild(it->second);
    m_mMaskSprite.erase(it);
}

void touchMap::setLevel(int level)
{
    if (m_level == level) return;
    
    clear();
    m_level = level;

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
    
    
    for (size_t i = 0; i < pMapCfg->vMapCell.size(); i++)
    {
        if (pMapCfg->vMapCell[i] == 0) continue;
        mapCell* pMapCell = getMapCell(i);
        if (! pMapCell) continue;
        pMapCell->setState(MC_STATE_BLOCK);
        createMask(i, 255, 0, 0);
    }
    
    for (size_t i = 0; i < pLevelCfg->vPropsPos.size(); i++) {
        if (pLevelCfg->vPropsPos[i].key >= m_mapCells->count()) continue;
        if (m_PathFinder->inBlockList(pLevelCfg->vPropsPos[i].key)) continue;
        m_pPropsManager->show(pLevelCfg->vPropsPos[i].key, pLevelCfg->vPropsPos[i].value);
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
        if (! m_PathFinder->inBlockList(p))
            pos->push_back(p);
        visballs->removeObjectAtIndex(index);
    }
	CC_SAFE_DELETE(visballs);
}

/////////////////props
void touchMap::maskMap(int pos, int maskType)
{
    mapCell* pMapCell = getMapCell(pos);
    if (! pMapCell) return;
    
    pMapCell->setState(maskType);
    
    int r = 0, g = 0, b = 0;
    switch (maskType) {
        case MC_STATE_MASK:
            b = 255;
            break;
        case MC_STATE_NOMOVE:
            g = 255;
            break;
        case MC_STATE_BLOCK:
            r = 255, g = 100;
            break;
        default:
            r = 255, g = 255;
            break;
    }
    createMask(pos, r, g, b);
}

void touchMap::disMaskMap(int pos)
{
    mapCell* pMapCell = getMapCell(pos);
    if (! pMapCell) return;
    
    deleteMask(pos);
    pMapCell->setState(MC_STATE_NORMAL);
    m_pPropsManager->hide(pos);
}

///////////////Script
int touchMap::script_destroyBall(lua_State* L)
{
    int p = m_pPropsManager->getSelectPos();
    ball* pBall = m_BallManager->getBall(p);
    if (! pBall) return 0;
    
    pBall->setVisible(false, false);
    m_pPropsManager->pick(0);
    return 1;
}

int touchMap::script_getOldSelect(lua_State* L)
{
    lua_pushnumber(L, m_BallManager->getOldSelectId());
    return 1;
}

int touchMap::script_getSelect(lua_State* L)
{
    lua_pushnumber(L, m_BallManager->getSelectId());
    return 1;
}

int touchMap::script_changePosition(lua_State* L)
{
	int result = 0;
	
	int paramCount = lua_gettop(L) - 2;
	if (paramCount != 2) {
        lua_pushboolean(L, result);
        return result;
    }
    
    int oldp = lua_tonumber(L, 3);
    int newp = lua_tonumber(L, 4);
    
    m_BallManager->changePosition(oldp, newp);
    
    result = 1;
    lua_pushboolean(L, result);
	return result;
    
}

int touchMap::script_disableSpecialBall(lua_State* L)
{
    
}

int touchMap::script_clearSelectProps(lua_State* L)
{
    m_pPropsManager->pick(0);
    return 1;
}

int touchMap::script_getBall(lua_State* L)
{
	int result = 0;
	
	int paramCount = lua_gettop(L) - 2;
	if (paramCount != 1) {
        lua_pushboolean(L, result);
        return result;
    }
    
    int p = lua_tonumber(L, 3);
    ball* pball = m_BallManager->getBall(p);
    if (pball && pball->isVisible()) {
        lua_pushlightuserdata(L, pball);
    }else{
        lua_pushboolean(L, false);
    }
    
    return 1;
}

int touchMap::script_moveto(lua_State* L)
{
	int result = 0;
	
	int paramCount = lua_gettop(L) - 2;
	if (paramCount != 2) {
        lua_pushboolean(L, result);
        return result;
    }
    
    int p = lua_tonumber(L, 3);
    ball* pselect = m_BallManager->getBall(m_BallManager->getSelectId());
    if (! pselect || ! pselect->isVisible()) return result;

    int flag = lua_tonumber(L, 4);
    if (flag) {
        ball* pball = m_BallManager->getBall(p);
        if (! pball || pball->isVisible()) return result;
        
        pball->setBallClass(pselect->getClassID());
        m_BallManager->show(pselect->getClassID(), p);
        m_BallManager->hide(pselect->getPos().x, pselect->getPos().y);
    }else{
        m_PathFinder->onMoveBall(m_BallManager->getKey(pselect->getPos().x, pselect->getPos().y), p);
    }
    
    return 1;
    
}


int touchMap::callFunction(lua_State* L)
{
	const char* funcName = lua_tostring(L, 2);
	if (strcmp(funcName, "destroyBall") == 0)
		return script_destroyBall(L);
    else if (strcmp(funcName, "getOldSelect") == 0)
        return script_getOldSelect(L);
    else if (strcmp(funcName, "getSelect") == 0)
        return script_getSelect(L);
    else if (strcmp(funcName, "changePosition") == 0)
        return script_changePosition(L);
    else if (strcmp(funcName, "disableSpecialBall") == 0)
        return script_disableSpecialBall(L);
    else if (strcmp(funcName, "clearSelectProps") == 0)
        return script_clearSelectProps(L);
    else if (strcmp(funcName, "getBall") == 0)
        return script_getBall(L);
    else if (strcmp(funcName, "moveto") == 0)
        return script_moveto(L);
    
    return 1;
    
}

