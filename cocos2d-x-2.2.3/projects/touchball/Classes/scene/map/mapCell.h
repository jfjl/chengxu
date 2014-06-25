//
//  mapCell.h
//  touchball
//
//  Created by carlor on 14-6-19.
//
//

#ifndef __touchball__mapCell__
#define __touchball__mapCell__

#include "cocos2d.h"

enum
{
    MC_STATE_NORMAL = 0,//正常
    MC_STATE_MASK   = 1,
    MC_STATE_NOMOVE = 2,
    MC_STATE_BLOCK  = 4,
    MC_STATE_FLAG   = 100,
};

typedef unsigned int MapCellState;

class mapCell : public cocos2d::CCSprite
{
public:
    mapCell();
    ~mapCell();

public:
    bool init(int x, int y);
    static mapCell* create(int x, int y);

    int getState();
    void setState(MapCellState value);
private:
    int m_posx;
    int m_posy;
    MapCellState m_state;
};

#endif /* defined(__touchball__mapCell__) */
