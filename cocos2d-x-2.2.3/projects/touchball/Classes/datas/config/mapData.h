#ifndef CFG_MAPDATA_H
#define CFG_MAPDATA_H

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "HawkUtil.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../hawksdk/HawkUtil.h"
#endif

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
