//
//  mapCell.cpp
//  touchball
//
//  Created by carlor on 14-6-19.
//
//

#include "mapCell.h"

mapCell::mapCell()
{
    
}

mapCell::~mapCell()
{
    
}

bool mapCell::init(int x, int y)
{
    if (! CCSprite::init()) {
        return false;
    }
    m_posx = x;
    m_posy = y;
    setState(MC_STATE_NORMAL);
    
    return true;
}

mapCell* mapCell::create(int x, int y)
{
    mapCell* pRet = new mapCell();
    if (! pRet->init(x, y)) {
        CC_SAFE_DELETE(pRet);
        return pRet;
    }
    
    pRet->autorelease();
    return pRet;
}

int mapCell::getState()
{
    return m_state;
}

void mapCell::setState(MapCellState value)
{
    m_state = value;
}
