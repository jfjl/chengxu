#ifndef CFG_BALLDATA_H
#define CFG_BALLDATA_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

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
	//出现形状条件
	int ShapeCondition;
	//出现个数条件
	int CountCondition;
};

#pragma pack(pop)

#endif
