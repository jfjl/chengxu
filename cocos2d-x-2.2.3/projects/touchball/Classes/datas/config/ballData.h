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
	//��id
	int BallId;
	//������
	int BallType;
	//�Ƿ�����ƶ�
	int CanMove;
	//�������Ϊ
	int PreBall;
};

#pragma pack(pop)

#endif
