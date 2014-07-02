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
, m_mapCells(NULL)
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
//    m_Backgound->setScale(TEXTURESCALE);
    m_Backgound->setAnchorPoint(ccp(0, 0));
    
    m_MapSize = CCSizeMake(m_Backgound->boundingBox().getMaxX()-m_Backgound->boundingBox().getMinX(),
                           m_Backgound->boundingBox().getMaxY()-m_Backgound->boundingBox().getMinY());
    
    this->addChild(m_Backgound);
    this->addChild(m_BallManager);
    this->setAnchorPoint(ccp(0, 0));
    
	m_Width = width;
	m_Height = height;
    
    m_mapCells = new CCArray();
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            mapCell* pMapCell = mapCell::create(j, i);
            m_mapCells->addObject(pMapCell);
        }
    }
    
    m_MarginX = 40;
    m_MarginY = 30;
    m_Margin = MAPCELL_SIZE - 8;
    
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
            m_BallManager->addChild(pBall);
            float s = m_Margin;// * TEXTURESCALE;
            int ballx = s * pBall->getPos().x + s / 2 + m_MarginX;
            int bally = s * pBall->getPos().y + s / 2 + m_MarginY;
            pBall->setPosition(ccp(ballx, bally));
//            pBall->setVisible(true, false);
			addEventLister(pBall);
		}
	}
    return true;
}

ball* ballMap::getBall(int x, int y)
{
    return m_BallManager->getBall(x, y);
}

mapCell* ballMap::getMapCell(int pos)
{
    return (mapCell*) m_mapCells->objectAtIndex(pos);
}

bool ballMap::inBlockList(int pos)
{
    mapCell* pMapCell = getMapCell(pos);
    if (! pMapCell) return false;
    
    return (pMapCell->getState() == MC_STATE_BLOCK);
}


int ballMap::getPosition(int x, int y)
{
    return y * getWidth() + x;
}
////////////////props
void ballMap::maskMap(int pos, int maskType)
{
    
}

void ballMap::disMaskMap(int pos)
{
    
}

