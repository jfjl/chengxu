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
	//������״����
	int ShapeCondition;
	//������
	int BallType;
	//���ָ�������
	int CountCondition;
};

#pragma pack(pop)

#endif
