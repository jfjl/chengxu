#ifndef CFG_AWARDBALLDATA_H
#define CFG_AWARDBALLDATA_H

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "HawkUtil.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../hawksdk/HawkUtil.h"
#endif

#pragma pack(push)
#pragma pack(1)

struct awardBallData
{
	//出现形状条件
	int ShapeCondition;
	//球种类
	int BallType;
	//出现个数条件
	int CountCondition;
};

#pragma pack(pop)

#endif
