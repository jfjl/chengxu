//
//  ballMap.h
//  ppball
//
//  Created by sora on 13-3-13.
//
//

#ifndef __ppball__ballMap__
#define __ppball__ballMap__

#include "ballManager.h"
#include "ScriptObject.h"
#include "mapCell.h"

class ballMap : public Object, public CCLayer
{
protected:
	CC_SYNTHESIZE(CCSprite*, m_Backgound, BackGround);
    
    //mapcell
    CC_SYNTHESIZE(CCArray, *m_mapCells, MapCells);

	virtual void addEventLister(CCObject *pball);

    int getPosition(int x, int y);
public:
	ballMap(void);
	~ballMap(void);
    
	CC_SYNTHESIZE_READONLY(int, m_Width, Width);
	CC_SYNTHESIZE_READONLY(int, m_Height, Height);
	CC_SYNTHESIZE_READONLY(int, m_MarginX, MarginX);
	CC_SYNTHESIZE_READONLY(int, m_MarginY, MarginY);
	CC_SYNTHESIZE_READONLY(int, m_Margin, Margin);
	CC_SYNTHESIZE_READONLY(ballManager*, m_BallManager, BallManager);
    CC_SYNTHESIZE_READONLY(CCSize, m_MapSize, MapSize);
    
	virtual bool init(const char*fileName, int width, int height);
	static ballMap* create(const char*fileName, int width, int height);
	virtual bool createBalls();

    ball* getBall(int x, int y);
    mapCell* getMapCell(int pos);
    bool inBlockList(int pos);
    //props
    virtual void maskMap(int pos, int maskType);
    virtual void disMaskMap(int pos);
    
	void next();
};

#endif /* defined(__ppball__ballMap__) */
