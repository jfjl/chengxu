//
//  ballMap.cpp
//  ppball
//
//  Created by carlor on 13-3-13.
//
//

#include "ballMap.h"

ballMap::ballMap(void)
: m_BallManager(NULL)
, m_Backgound(NULL)
, m_Width(0)
, m_Height(0)
{
    
}

ballMap::~ballMap(void)
{
	CC_SAFE_DELETE(m_Backgound);
	CC_SAFE_DELETE(m_BallManager);
}

bool ballMap::init(const char*fileName, int width, int height)
{
    m_BallManager = ballManager::create(width, height);
	m_Backgound  = CCSprite::createWithSpriteFrameName(fileName);
    m_Backgound->setScale(TEXTURESCALE);
    m_Backgound->setAnchorPoint(ccp(0, 0));
    
    m_MapSize = CCSizeMake(m_Backgound->boundingBox().getMaxX()-m_Backgound->boundingBox().getMinX(),
                           m_Backgound->boundingBox().getMaxY()-m_Backgound->boundingBox().getMinY());
    
    this->addChild(m_Backgound);
    this->setAnchorPoint(ccp(0, 0));
    
	m_Width = width;
	m_Height = height;
    
	setTouchEnabled(false);
	setKeypadEnabled(false);
    
    //createBalls();
	return true;
}

ballMap* ballMap::create(const char*fileName, int width, int height)
{
	ballMap *pRet = new ballMap();
	pRet->init(fileName, width, height);
	pRet->autorelease();
	return pRet;
}

void ballMap::addEventLister(CCObject *pball)
{

}

bool ballMap::createBalls()
{
	for (int i = 0; i < m_Height; i++){
		for (int j = 0; j < m_Width; j++){
            ball *pBall = ball::create(MAGICBALL_ID, CCPointMake(j, i));
			m_BallManager->add(pBall);
            this->addChild(pBall);
            float s = MAPCELL_SIZE * TEXTURESCALE;
            int ballx = s * pBall->getPos().x + s / 2;
            int bally = s * pBall->getPos().y + s / 2;
            pBall->setPosition(ccp(ballx, bally));
			addEventLister(pBall);
		}
	}
    return true;
}

ball* ballMap::getBall(int x, int y)
{
    return m_BallManager->getBall(x, y);
}
