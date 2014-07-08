//
//  pathFinder.h
//  ppball
//
//  Created by carlor on 13-3-16.
//
//

#ifndef __ppball__pathFinder__
#define __ppball__pathFinder__

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../map/ballMap.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "ballMap.h"
#endif

class pathNode : public CCObject
{
public:
    pathNode(void);
    ~pathNode(void);
    
    CC_SYNTHESIZE(int, m_F, F);
    CC_SYNTHESIZE_READONLY(int, m_PosKey, PosKey);
    CC_SYNTHESIZE(int, m_G, G);
    CC_SYNTHESIZE(int, m_H, H);
    CC_SYNTHESIZE(int, m_ParentKey, ParentKey);
    
    bool init(int posKey);
    static pathNode* create(int posKey);
};

class pathFinder
{
private:
    CC_SYNTHESIZE(ballMap, *m_Owner, Owner);
    CC_SYNTHESIZE(CCDictionary, *m_CloseList, CloseList);
    CC_SYNTHESIZE(CCDictionary, *m_OpenList, OpenList);
    CC_SYNTHESIZE(vector<int>, m_PathArray, PathArray);
    CC_SYNTHESIZE(CCSize, *m_MapSize, MapSize);
    
    void clear();
    
    int getPosKey(int x, int y);
    int getOrder(CCArray *path, pathNode *node);
    int getG(int fx, int fy, int tx, int ty);
    int getG(pathNode* parentNode);
    int getH(int fx, int fy);
    int compareF(pathNode *node1, pathNode *node2);
    
    pathNode* addOpenNode(int posKey, int g, int h, int parentKey);
    void removeOpenNode(int posKey);
    pathNode* addCloseNode(int posKey,  int g, int h, int parentKey);
    void removeCloseNode(int posKey);
    
    bool moveBall(int srcPosKey, int destPosKey);
    bool toMoveBall(int srcPosKey, int destPosKey);
    void collectMovedPos(int destPosKey);
    
public:
    pathFinder(void);
    ~pathFinder(void);
    
    CC_SYNTHESIZE(int, m_DestPosition, DestPosition);
    
    bool init(ballMap* owner, int widht, int height);
//    void initBlockList(int level);
    static pathFinder* create(ballMap* owner, int width, int height);
    
    bool inBlockList(int p);
    bool inCloselist(int p);
    pathNode* inOpenList(int p);

    bool onMoveBall(int srcPosKey, int destPosKey);
    int getNextNode();
//private:
//    CCDictionary* m_mBlackList;
};

#endif /* defined(__ppball__pathFinder__) */
