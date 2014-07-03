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
	pRet->autorelease();
    return pRet;
}


////////////////////////
pathFinder::pathFinder(void)
    : m_CloseList(NULL)
    , m_OpenList(NULL)
    , m_MapSize(NULL)
    , m_DestPosition(0)
    , m_Owner(NULL)
{
}

pathFinder::~pathFinder(void)
{
    clear();
    CC_SAFE_DELETE(m_MapSize);
    CC_SAFE_DELETE(m_OpenList);
    CC_SAFE_DELETE(m_CloseList);
}

void pathFinder::clear()
{
    m_CloseList->removeAllObjects();
    m_OpenList->removeAllObjects();
}

bool pathFinder::init(ballMap* owner, int width, int height)
{
    m_Owner = owner;
    
    m_MapSize = new CCSize(width, height);
    m_CloseList = CCDictionary::create();
    m_CloseList->retain();
    m_OpenList  = CCDictionary::create();
    m_OpenList->retain();
    m_PathArray.clear();
    
    setDestPosition(-1);
    
    return true;
}
/*
void pathFinder::initBlockList(int level)
{
    const levelCfg* pLevelCfg = g_clientData->getLevelCfg(level);
    if (! pLevelCfg)
        return;
    
    const mapCfg* pMapCfg = g_clientData->getMapCfg(pLevelCfg->MapId);
    if (! pMapCfg)
        return;
    
    m_mBlackList->removeAllObjects();
    for (size_t i = 0; i < pMapCfg->vMapCell.size(); i++)
    {
        if (pMapCfg->vMapCell[i] == 0) continue;
        
        pathNode *pNode = pathNode::create(i);
        m_mBlackList->setObject(pNode, i);
    }
}
*/
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

int pathFinder::getG(pathNode* parentNode)
{
    if (parentNode) {
        return parentNode->getG() + 10;
    }
    
    return 10;
}

int pathFinder::getH(int fx, int fy)
{
    int width = (int) getMapSize().width;
//    int height= (int) getMapSize().height;

    int tx  = m_DestPosition % width;
    int ty  = floor((double)m_DestPosition / width);
    return abs(tx - fx) + abs(ty - fy) * 10;
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

void pathFinder::removeOpenNode(int posKey)
{
    m_OpenList->removeObjectForKey(posKey);
}

pathNode* pathFinder::addCloseNode(int posKey, int g, int h, int parentKey)
{
    if (m_OpenList->objectForKey(posKey) != NULL || m_CloseList->objectForKey(posKey))
        return NULL;
    else{
        pathNode *pRet = pathNode::create(posKey);
        pRet->setG(g);
        pRet->setH(h);
        pRet->setF(g + h);
        pRet->setParentKey(parentKey);
        m_CloseList->setObject(pRet, posKey);
        
        return pRet;
    }
}

void pathFinder::removeCloseNode(int posKey)
{
    m_CloseList->removeObjectForKey(posKey);
}

int pathFinder::getOrder(CCArray *path, pathNode *node)
{
    for (int i = 0; i < path->count(); i++){
        if (compareF(dynamic_cast<pathNode*>(path->objectAtIndex(i)), node) > 0){
            path->insertObject(node, i);
            return i;
        }
    }
    
    path->addObject(node);
    return path->count() - 1;
}

bool pathFinder::inBlockList(int p)
{
    return m_Owner->inBlockList(p);
}

bool pathFinder::inCloselist(int p)
{
    return m_CloseList->objectForKey(p);
}

pathNode* pathFinder::inOpenList(int p)
{
    return (pathNode*) m_OpenList->objectForKey(p);
}

bool compare(const pathNode* a, const pathNode* b)
{
    return (a->getF() < b->getF());
}

bool pathFinder::moveBall(int srcPosKey, int destPosKey)
{
    pathNode* ptemp = inOpenList(srcPosKey);
    if (! ptemp) return false;
    
    int tg = ptemp->getG();
    int th = ptemp->getH();
    int tp = ptemp->getParentKey();
    removeOpenNode(srcPosKey);
    addCloseNode(srcPosKey, tg, th, tp);
    
    if (srcPosKey == destPosKey) return true;
    
    int width = (int) getMapSize().width;
    int height= (int) getMapSize().height;
    
    int srcx  = srcPosKey % width;
    int srcy  = floor((double)srcPosKey / width);
    int destx = destPosKey % width;
    int desty = floor((double)destPosKey / width);

    for (int i = srcy - 1; i <= srcy + 1; i++) {
        if (i < 0 || i >= height) continue;
        for (int j = srcx - 1; j <= srcx + 1; j++) {
            if (j < 0 || j >= width) continue;
            if (j != srcx && i != srcy) continue;
            
            int key = i * width + j;
            if (key == srcPosKey) continue;
            int gv = getG(j, i, destx, desty);
            int hv = getH(j, i);
            if (key == destPosKey) {
                addCloseNode(key, gv, hv, srcPosKey);
                return true;
            }
            
            ball* temp = m_Owner->getBall(j, i);
            if (temp->isVisible()) continue;
            if (inCloselist(key)) continue;
            if (inBlockList(key)) continue;
            
            pathNode* pnode = inOpenList(key);
            if (pnode != NULL) {
                if (pnode->getF() > gv + hv) {
                    pnode->setParentKey(srcPosKey);
                    pnode->setG(gv);
                    pnode->setH(hv);
                }
            }else{
                addOpenNode(key, gv, hv, srcPosKey);
            }
        }
    }
    
    vector<pathNode*> nodes;
	CCDictElement *pElement;
	CCDICT_FOREACH(m_OpenList, pElement) {
		pathNode* pnode = (pathNode*) pElement->getObject();
        nodes.push_back(pnode);
    }
    sort(nodes.begin(), nodes.end(), compare);
    for (int i = 0; i < nodes.size(); i++) {
        if (moveBall(nodes[i]->getPosKey(), destPosKey))
            return true;
    }
    
    return false;
/*
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
        if (inBlockList(srcy * width + i)) continue;
        
        int posKey = getPosKey(i, srcy);
        pathNode *pPathNode = addOpenNode(posKey, getG(i, srcy, destx, desty), getH(i, srcy), srcPosKey);
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
        if (inBlockList(i * width + srcx)) continue;
        int posKey = getPosKey(srcx, i);
        pathNode *pPathNode = addOpenNode(posKey, getG(srcx, i, destx, desty), getH(srcx, i), srcPosKey);
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

    while (temp->count()) {
        int index = -1;
        pathNode* minfNode;
        for (int i = temp->count() - 1; i >= 0; i--)
        {
            pathNode* pPathNode = dynamic_cast<pathNode *>(temp->objectAtIndex(i));
            if (! pPathNode) continue;
            if ((index >= 0) && (pPathNode->getF() >= minfNode->getF())) continue;
            index = i;
            minfNode = pPathNode;
        }
        if (minfNode) {
            if (moveBall(minfNode->getPosKey(), destPosKey)) {
                temp->removeAllObjects();
                CC_SAFE_DELETE(temp);
                return true;
            }
            else
            {
                temp->removeObject(minfNode);
            }
        }
	}
    temp->removeAllObjects();
	CC_SAFE_DELETE(temp);

    return false;
 */
}

void pathFinder::collectMovedPos(int destPosKey)
{
    CCObject *obj = m_CloseList->objectForKey(destPosKey);
//    if (obj == NULL) obj = m_OpenList->objectForKey(destPosKey);
    
    if (obj == NULL) return;
    
    pathNode *node = dynamic_cast<pathNode*>(obj);
    if (node->getParentKey() >= 0){
        m_PathArray.push_back(destPosKey);
        collectMovedPos(node->getParentKey());
    }
    
}

bool pathFinder::toMoveBall(int srcPosKey, int destPosKey)
{
//    CCLog("from p = %d", srcPosKey);
//    CCLog("to p = %d", destPosKey);
    addOpenNode(srcPosKey, 0, 0, -1);

    if (moveBall(srcPosKey, destPosKey)){
        collectMovedPos(destPosKey);
        m_OpenList->removeAllObjects();
        m_CloseList->removeAllObjects();
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
    if (m_PathArray.size() > 0){
        int result = m_PathArray[m_PathArray.size() - 1];
        m_PathArray.erase(m_PathArray.begin() + m_PathArray.size() - 1);
        
        return result;
    }else
        return -1;
}




