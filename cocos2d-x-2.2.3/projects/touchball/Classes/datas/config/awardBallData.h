#ifndef CFG_AWARDBALLDATA_H
#define CFG_AWARDBALLDATA_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

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
