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

class ballMap : public CCLayer
{
protected:
	CC_SYNTHESIZE(CCSprite*, m_Backgound, BackGround);

	virtual void addEventLister(CCObject *pball);
public:
	ballMap(void);
	~ballMap(void);
    
	CC_SYNTHESIZE_READONLY(int, m_Width, Width);
	CC_SYNTHESIZE_READONLY(int, m_Height, Height);
	CC_SYNTHESIZE_READONLY(ballManager*, m_BallManager, BallManager);
    CC_SYNTHESIZE_READONLY(CCSize, m_MapSize, MapSize);
    
	virtual bool init(const char*fileName, int width, int height);
	static ballMap* create(const char*fileName, int width, int height);
	virtual bool createBalls();

    ball* getBall(int x, int y);
    
	void next();
};

#endif /* defined(__ppball__ballMap__) */
