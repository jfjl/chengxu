//
//  pathFinder.cpp
//  ppball
//
//  Created by sora on 13-3-16.
//
//

#include "pathFinder.h"
#include "ClientData.h"
#include <math.h>

pathNode::pathNode(void)
    : m_PosKey(-1)
    , m_G(-1)
    , m_H(-1)
    , m_F(-1)
    , m_ParentKey(-1)
{
    
}

pathNode::~pathNode(void)
{
    //std::cout<<"ddddd"<<std::endl;
}

bool pathNode::init(int posKey)
{
    m_PosKey = posKey;
    
    return true;
}

pathNode* pathNode::create(int posKey)
{
    pathNode *pRet = new pathNode();
    pRet->init(posKey);
	pRet->retain();
	pRet->autorelease();
    return pRet;
}


////////////////////////
pathFinder::pathFinder(void)
    : m_CloseList(NULL)
    , m_OpenList(NULL)
    , m_PathArray(NULL)
    , m_MapSize(NULL)
    , m_DestPosition(0)
{
    
}

pathFinder::~pathFinder(void)
{
    clear();
    CC_SAFE_DELETE(m_MapSize);
    CC_SAFE_DELETE(m_OpenList);
    CC_SAFE_DELETE(m_CloseList);
    CC_SAFE_DELETE(m_PathArray);
}

void pathFinder::clear()
{
    m_CloseList->removeAllObjects();
    m_OpenList->removeAllObjects();
    m_PathArray->removeAllObjects();
    m_mBlackList.clear();
}

bool pathFinder::init(ballMap* owner, int width, int height)
{
    m_Owner = owner;
    
    m_MapSize = new CCSize(width, height);
    m_CloseList = CCDictionary::create();
    m_CloseList->retain();
    m_OpenList  = CCDictionary::create();
    m_OpenList->retain();
    m_PathArray = CCArray::create();
    m_PathArray->retain();
    
    setDestPosition(-1);
    
    return true;
}

void pathFinder::initBlackList(int level)
{
    m_mBlackList.clear();
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(level);
    if (! pLevelCfg)
        return;
    
    const mapCfg* pMapCfg = g_clientData->getMapCfg(pLevelCfg->MapId);
    if (! pMapCfg)
        return;
    
    for (size_t i = 0; i < pMapCfg->vMapCell.size(); i++)
    {
        if (pMapCfg->vMapCell[i] == 0) continue;
        
        m_mBlackList[i] = true;
    }
}

pathFinder* pathFinder::create(ballMap* owner, int width, int height)
{
    pathFinder* pRet = new pathFinder();
    pRet->init(owner, width, height);
    return pRet;
}

int pathFinder::getPosKey(int x, int y)
{
    return y * (int) getMapSize().width + x;
}

int pathFinder::getG(int fx, int fy, int tx, int ty)
{
    return (abs(fx - tx) + abs(fy - ty)) * 10;
}

int pathFinder::compareF(pathNode *node1, pathNode *node2)
{
    int f1 = node1->getF();
    int f2 = node2->getF();

    return f1 > f2 ? 1 : (f1 < f2 ? -1 : 0);
}


pathNode* pathFinder::addOpenNode(int posKey, int g, int h, int parentKey)
{
    if (m_OpenList->objectForKey(posKey) != NULL || m_CloseList->objectForKey(posKey))
        return NULL;
    else{
        pathNode *pRet = pathNode::create(posKey);
        pRet->setG(g);
        pRet->setH(h);
        pRet->setF(g + h);
        pRet->setParentKey(parentKey);
        m_OpenList->setObject(pRet, posKey);
        
        return pRet;
    }
}

pathNode* pathFinder::addCloseNode(int posKey)
{
    CCObject *pRet = m_OpenList->objectForKey(posKey);
    if (pRet == NULL) return NULL;
    
    m_CloseList->setObject(pRet, posKey);
    
    m_OpenList->removeObjectForKey(posKey);
    
    return dynamic_cast<pathNode*>(pRet);
}

int pathFinder::getOrder(CCArray *path, pathNode *node)
{
    CCObject *obj = NULL;
    for (int i = 0; i < path->count(); i++){
        if (compareF(dynamic_cast<pathNode*>(path->objectAtIndex(i)), node) > 0){
            path->insertObject(node, i);
            return i;
        }
    }
    
    path->addObject(node);
    return path->count() - 1;
}

bool pathFinder::inBlackList(int p)
{
    map<int, bool>::iterator it = m_mBlackList.find(p);
    if (it == m_mBlackList.end())
        return false;
    return true;
}


bool pathFinder::moveBall(int srcPosKey, int destPosKey)
{
    int width = (int) getMapSize().width;
    int height= (int) getMapSize().height;
    
    int srcx  = srcPosKey % width;

    int srcy  = floor((double)srcPosKey / width);
    int destx = destPosKey % width;
    int desty = floor((double)destPosKey / width);

    CCArray* temp = new CCArray();
    
    for (int i = MAX(0, srcx - 1); i <= MIN(width - 1, srcx + 1); i++) {
        if (i == srcx) continue;
        
        ball *destBall = m_Owner->getBall(i, srcy);
        if (destBall->isVisible()) continue;
        if (inBlackList(srcy * m_MapSize->width + i)) continue;
        
        int posKey = getPosKey(i, srcy);
        pathNode *pPathNode = addOpenNode(posKey, 10, getG(i, srcy, destx, desty), srcPosKey);
        if (pPathNode == NULL) continue;
        
        if (i == destx && srcy == desty){
			temp->removeAllObjects();
			CC_SAFE_DELETE(temp);
            return addCloseNode(posKey) != NULL;
		}else
            getOrder(temp, pPathNode);
    }
    
    for (int i = MAX(0, srcy - 1); i <= MIN(height - 1, srcy + 1); i++) {
        if (i == srcy) continue;
        
        ball *destBall = m_Owner->getBall(srcx, i);
        if (destBall->isVisible()) continue;
        
        int posKey = getPosKey(srcx, i);
        pathNode *pPathNode = addOpenNode(posKey, 10, getG(srcx, i, destx, desty), srcPosKey);
        if (pPathNode == NULL) continue;
        
        if (i == desty && srcx == destx){
			temp->removeAllObjects();
			CC_SAFE_DELETE(temp);
            return addCloseNode(posKey) != NULL;
		}else{
            getOrder(temp, pPathNode);
        }
    }
    
    addCloseNode(srcPosKey);
    
    for (int i = 0; i < temp->count(); i++)
    {
        pathNode *pPathNode = dynamic_cast<pathNode *>(temp->objectAtIndex(i));
        if (moveBall(pPathNode->getPosKey(), destPosKey)){
			temp->removeAllObjects();
			CC_SAFE_DELETE(temp);

            return true;
		}
    }
	temp->removeAllObjects();
	CC_SAFE_DELETE(temp);

    return false;
}

void pathFinder::collectMovedPos(int destPosKey)
{
    CCObject *obj = m_CloseList->objectForKey(destPosKey);
    if (obj == NULL) obj = m_OpenList->objectForKey(destPosKey);
    
    if (obj == NULL) return;
    
    pathNode *node = dynamic_cast<pathNode*>(obj);
    if (node->getParentKey() >= 0){
        m_CloseList->removeObjectForKey(destPosKey);
        m_OpenList->removeObjectForKey(destPosKey);
        m_PathArray->addObject(node);
        collectMovedPos(node->getParentKey());
    }
}

bool pathFinder::toMoveBall(int srcPosKey, int destPosKey)
{
    addOpenNode(srcPosKey, 0, 0, -1);

    if (moveBall(srcPosKey, destPosKey)){
        collectMovedPos(destPosKey);
        return true;
    }
    
    return false;
}

bool pathFinder::onMoveBall(int srcPosKey, int destPosKey)
{
    clear();
    setDestPosition(destPosKey);
    return toMoveBall(srcPosKey, destPosKey);
}

int pathFinder::getNextNode()
{
    if (m_PathArray->count() > 0){
        pathNode *node = dynamic_cast<pathNode*>(m_PathArray->lastObject());
        m_PathArray->removeLastObject();
        int posKey = node->getPosKey();
        CC_SAFE_DELETE(node);
        return posKey;
    }else
        return -1;
}




