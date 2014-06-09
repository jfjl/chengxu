#ifndef CFG_MAPDATA_H
#define CFG_MAPDATA_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

#pragma pack(push)
#pragma pack(1)

struct mapData
{
	//地图id
	int MapId;
	//地图格子
	unsigned char MapCell[256];
};

#pragma pack(pop)

#endif
