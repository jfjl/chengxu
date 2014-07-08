#ifndef CFG_LEVELDATA_H
#define CFG_LEVELDATA_H

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "HawkUtil.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../hawksdk/HawkUtil.h"
#endif

#pragma pack(push)
#pragma pack(1)

struct levelData
{
	//关卡
	int Level;
	//地图id
	int MapId;
	//1星需要分数
	int Score1;
	//2星需要分数
	int Score2;
	//3星需要分数
	int Score3;
	//连成几个可以消除
	int RemoveCount;
	//出现的球种类
	unsigned char BallIds[64];
	//特殊球初始化出现几率
	unsigned char SpecialBallIds[128];
	//多连奖励道具
	unsigned char RewardsProps[32];
	//可以使用的道具
	unsigned char CanUseProps[32];
	//万能球出现几率
	int MagicBallRate;
	//初始出现个数
	int InitCount;
	//遮罩道具出现位置
	unsigned char PropsPos[128];
	//限制步数
	int MaxStep;
};

#pragma pack(pop)

#endif
