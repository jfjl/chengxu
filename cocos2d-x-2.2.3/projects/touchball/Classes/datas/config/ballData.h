#ifndef CFG_BALLDATA_H
#define CFG_BALLDATA_H

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "HawkUtil.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../hawksdk/HawkUtil.h"
#endif

#pragma pack(push)
#pragma pack(1)

struct ballData
{
	//球id
	int BallId;
	//球种类
	int BallType;
	//是否可以移动
	int CanMove;
	//消除后变为
	int PreBall;
};

#pragma pack(pop)

#endif
