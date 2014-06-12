//
//  ballManager.cpp
//  ppball
//
//  Created by carlor on 13-3-13.
//
//

#include "ballManager.h"
#include "ClientData.h"

ballManager::ballManager(void)
: m_BallList(NULL)
, m_SelectId(-1)
{
}


ballManager::~ballManager(void)
{
	clear();
}

void ballManager::clear()
{
	for(std::vector<ball*>::iterator nball = m_BallList->begin(); nball != m_BallList->end(); ++nball){
		delete *nball;
		*nball = NULL;
	}
    
	m_BallList->clear();
}

bool ballManager::init(int width, int height)
{
	m_BallList = new ballVector(0);//width * height);
	m_seed = width;
	m_Width = width;
	m_Height = height;
    
	return true;
}

ballManager* ballManager::create(int width, int height)
{
	ballManager *pRet = new ballManager();
	pRet->init(width, height);
	return pRet;
}

int ballManager::getKey(int x, int y)
{
	int key = y * m_seed + x;
	return key >= m_BallList->size() ? -1 : key;
}

ball* ballManager::getBall(int x, int y)
{
	int key = getKey(x, y);
	if (key < 0) return NULL;
    
	ball *pRet = m_BallList->at(key);
	return pRet;
}

ball* ballManager::getBall(int pos)
{
    if (pos < 0 || pos >= m_BallList->size()) return NULL;
    
    return m_BallList->at(pos);
}

void ballManager::getVisibleBall(bool vis, CCArray *balls)
{
    for (int i = 0; i < m_BallList->size(); i++){
        ball *pBall = m_BallList->at(i);
        if (pBall->isVisible() == vis)
            balls->addObject(pBall);
    }
}

int ballManager::add(ball* value)
{
	int count = m_BallList->size();
	m_BallList->push_back(value);
    
    value->setVisible(false, false);
	value->setID(count);
	value->setBallState(BallStateNormal);
    value->setScale(TEXTURESCALE);
        
    return count;
}

bool ballManager::checkMove(int x, int y)
{
    return true;
}

int ballManager::select(int x, int y)
{
	ball *pball = getBall(x, y);
	if (pball == NULL) return -1;
	if (! pball->isVisible()) return -1;
    const ballCfg* pBallCfg = g_clientData->getBallCfg(pball->getClassID());
    if (pBallCfg && ! pBallCfg->CanMove) return -1;
    
    
    ball *pOldBall = getBall(m_SelectId);
    if (pOldBall != NULL){
        pOldBall->setBallState(BallStateNormal);
    }
    
	pball->setBallState(BallStateSelected);
    m_SelectId = pball->getID();
	return pball->getID();
}

int ballManager::show(int classId, int x, int y)
{
    return show(classId, getKey(x, y));
}

int ballManager::show(int classId, int posKey)
{
    ball *pball = getBall(posKey);
    if (pball == NULL) return -1;

	if (! pball->isVisible())
    {
		pball->setVisible(true, false);
    }
    pball->setBallState(BallStateNormal);
	pball->setBallClass(classId);
	return pball->getID();
}

int ballManager::hide(int x, int y)
{
    return hide(x, y);
}

int ballManager::hide(int posKey)
{
	ball *pball = getBall(posKey);
	if (pball == NULL) return -1;
    
	if (pball->isVisible())
    {
		pball->setVisible(false, false);
    }
	return pball->getID();
}

int ballManager::playShow(int classId, int x, int y)
{
	ball *pball = getBall(x, y);
	if (pball == NULL) return -1;
	
	if (! pball->isVisible())
    {
		pball->setVisible(true, true);
    }
	pball->setBallClass(classId);
	
    return pball->getID();
}

int ballManager::playHide(int x, int y)
{
    ball *pball = getBall(x, y);
    if (pball == NULL) return -1;
    
    if (pball->isVisible())
    {
        pball->setVisible(false, true);
    }
    return pball->getID();
}

int ballManager::playHide(ball *pball)
{
	if (pball->isVisible())
    {
		pball->setVisible(false, true, false);
    }
	return pball->getID();
}

bool ballManager::compare(ball *pball, int ClassID)
{
	if (pball->getClassID() == 0) return true;
    const ballCfg* pBallCfg = g_clientData->getBallCfg(pball->getClassID());
    if (pBallCfg)
    {
        return pBallCfg->nBasicBall == ClassID;
    }
	return pball->getClassID() == ClassID;
}

CCArray* ballManager::collectRemoveBalls(int ClassID, int x, int y, int ox, int oy)
{
	CCArray *result = new CCArray();

	ball *pball = getBall(x, y);
	if (! compare(pball, ClassID)) return result;

	int tx = x;
	int ty = y;
	ball *tempball = getBall(tx-ox, ty-oy);
	while((tx - ox >= 0 && ty - oy >= 0 && tx - ox < this->getWidth() && ty - oy < this->getHeight() && tempball != NULL && tempball->isVisible())){
		if (compare(tempball, ClassID)){
			result->addObject(tempball);
			tx -= ox;
			ty -= oy;
			tempball = getBall(tx-ox, ty-oy);
		}else
			break;
	}

	tx = x;
	ty = y;
	tempball = getBall(tx+ox, ty+oy);
	while((tx + ox < this->getWidth() && ty + oy < this->getHeight() && tx + ox >= 0 && ty + oy >= 0 && tempball != NULL && tempball->isVisible())){
		if (compare(tempball, ClassID)){
			result->addObject(tempball);
			tx += ox;
			ty += oy;
			tempball = getBall(tx+ox, ty+oy);
		}else
			break;
	}

	return result;
}

int ballManager::getRemoveShape(int curShape, int removeCount, int curType, std::vector<int>& vShapes)
{
    int shape = curShape == 0 ? AWARDBALL_SHAPE_COL : (curShape == 1 ? AWARDBALL_SHAPE_ROW : AWARDBALL_SHAPE_X);
    const awardBallCfg* pAwardBallCfg = g_clientData->getAwardBallCfg(shape);
    if (pAwardBallCfg && removeCount >= 3)//pAwardBallCfg->CountCondition)
    {
        if (vShapes.size() > 0) shape = AWARDBALL_SHAPE_MULTSHAPE;
        for (size_t j = 0; j < vShapes.size(); j++) {
            if (vShapes[j] != curType)
            {
                shape = AWARDBALL_SHAPE_MULTTYPE;
                break;
            }
        }
        vShapes.push_back(shape);
        return shape;
    }
    
    
    return 0;
}


CCArray* ballManager::checkRemove(ball* pball, int removeCount, int& nshape)
{
    const map<int, ballCfg> ballList = g_clientData->getBallList();
	CCArray *result = new CCArray();
    CCPoint pos = pball->getPos();
    
    vector<int> shapes;
    int shape = 0;
	for (int i = 0; i < 4; i++){
		CCPoint p = BALLREMOVEDIRS[i];
        for (map<int, ballCfg>::const_iterator it = ballList.begin(); it != ballList.end(); it++)
        {
            if (! it->second.BallType) continue; //不是万能球
            if (it->second.nBasicBall != it->second.BallId) continue;
            
			CCArray *dirRemove = collectRemoveBalls(it->second.nBasicBall, pos.x, pos.y, p.x, p.y);
			if (dirRemove->count() >= removeCount - 1) {
                int temp = getRemoveShape(i, dirRemove->count()+1, it->second.nBasicBall, shapes);
				if (temp > shape) shape = temp;
                for (int i = 0; i < dirRemove->count(); i++) {
                    ball* tempball = (ball*) dirRemove->objectAtIndex(i);
                    if (result->containsObject(tempball)) continue;
                    result->addObject(tempball);
                }
			}
			dirRemove->removeAllObjects();
			CC_SAFE_DELETE(dirRemove);
			
		}
	}
    if (shape > 0)
    {
        nshape = shape;
    }
    
	return result;
}

void ballManager::getRemoveBall(CCArray* balls)
{
    m_mCheckBalls.clear();
    int index = 0;
    while (index < balls->count()) {
        index++;
		ball *temp = (ball*) balls->objectAtIndex(index-1);
        
        const ballCfg* pballCfg = g_clientData->getBallCfg(temp->getClassID());
        if (! pballCfg) continue;
        const awardBallCfg* pawardBallCfg = g_clientData->getAwardBallCfgByBallType(pballCfg->BallType);
        if (! pawardBallCfg) continue;
        
        map<ball*, int>::iterator it = m_mCheckBalls.find(temp);
        if (it != m_mCheckBalls.end()) continue;
        
        m_mCheckBalls[temp] = 1;
        
        switch (pballCfg->BallType) {
            case BALLTYPE_ROW:
                getRemoveBallByRow(temp);
                break;
            case BALLTYPE_COL:
                getRemoveBallByCol(temp);
                break;
            case BALLTYPE_X:
                getRemoveBallByX(temp);
                break;
            case BALLTYPE_AROUND:
                getRemoveBallByAround(temp);
                break;
            case BALLTYPE_SAME:
                getRemoveBallBySame(temp);
                break;
        }
        
        for (int i = 0; i < m_vRemoveBalls.size(); i++) {
            if (balls->containsObject(m_vRemoveBalls[i])) continue;
            balls->addObject(m_vRemoveBalls[i]);
        }
    }
}

void ballManager::getRemoveBallByRow(ball* pball)
{
    m_vRemoveBalls.clear();
    int px = pball->getPos().x;
    int py = pball->getPos().y;
    
    for (int i = 0; i < GAMEMAPSIZE_WIDTH; i++) {
        if (px == i) continue;
        ball* temp = getBall(i, py);
        if (! temp || ! temp->isVisible()) continue;
        
        m_vRemoveBalls.push_back(temp);
    }
}

void ballManager::getRemoveBallByCol(ball* pball)
{
    m_vRemoveBalls.clear();
    int px = pball->getPos().x;
    int py = pball->getPos().y;
    for (int i = 0; i < GAMEMAPSIZE_HEIGHT; i++) {
        if (py == i) continue;
        ball* temp = getBall(px, i);
        if (! temp || ! temp->isVisible()) continue;
        
        m_vRemoveBalls.push_back(temp);
    }
}

void ballManager::getRemoveBallByX(ball* pball)
{
    m_vRemoveBalls.clear();
    int px = pball->getPos().x;
    int py = pball->getPos().y;
    for (int i = 0; i < 4; i++) {
        px = px + BALLXREMOVEDIRS[i].x;
        py = py + BALLXREMOVEDIRS[i].y;
        while (px >= 0 && px < GAMEMAPSIZE_WIDTH && py >= 0 && py < GAMEMAPSIZE_HEIGHT) {            
            ball* temp = getBall(px, py);
            
            px = px + BALLXREMOVEDIRS[i].x;
            py = py + BALLXREMOVEDIRS[i].y;

            if (! temp || ! temp->isVisible()) continue;
            
            m_vRemoveBalls.push_back(temp);
        }
    }
}

void ballManager::getRemoveBallByAround(ball* pball)
{
    m_vRemoveBalls.clear();
    int px = pball->getPos().x;
    int py = pball->getPos().y;
    for (int i = 0; i < 8; i++)
    {
        px = px + BALLAROUNDS[i].x;
        py = py + BALLAROUNDS[i].y;
        if (px >= 0 && px < GAMEMAPSIZE_WIDTH && py >= 0 && py < GAMEMAPSIZE_HEIGHT) {
            ball* temp = getBall(px, py);
            if (! temp || ! temp->isVisible()) continue;
            
            m_vRemoveBalls.push_back(temp);
        }
    }
}

void ballManager::getRemoveBallBySame(ball* pball)
{
    m_vRemoveBalls.clear();
    const ballCfg* pballCfg = g_clientData->getBallCfg(pball->getClassID());
    if (! pballCfg) return;
    
    int basicId = pballCfg->nBasicBall;
    
    for (int i = 0; i < GAMEMAPSIZE_WIDTH; i++)
    {
        for (int j = 0; j < GAMEMAPSIZE_HEIGHT; j++)
        {
            ball* temp = getBall(i, j);
            if (! temp || ! temp->isVisible()) continue;
            
            pballCfg = g_clientData->getBallCfg(temp->getClassID());
            if (! pballCfg) continue;
            if (pballCfg->nBasicBall == basicId) continue;
            
            m_vRemoveBalls.push_back(temp);
        }
    }
    
    return;
}
