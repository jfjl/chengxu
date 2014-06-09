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
	//��id
	int BallId;
	//������
	int BallType;
	//�Ƿ�����ƶ�
	int CanMove;
	//�������Ϊ
	int PreBall;
	//������״����
	int ShapeCondition;
	//���ָ�������
	int CountCondition;
};

#pragma pack(pop)

#endif
