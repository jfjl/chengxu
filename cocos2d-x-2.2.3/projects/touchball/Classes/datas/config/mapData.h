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
	//��ͼid
	int MapId;
	//��ͼ����
	unsigned char MapCell[256];
};

#pragma pack(pop)

#endif
