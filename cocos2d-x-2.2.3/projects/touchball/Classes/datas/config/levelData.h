#ifndef CFG_LEVELDATA_H
#define CFG_LEVELDATA_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

#pragma pack(push)
#pragma pack(1)

struct levelData
{
	//�ؿ�
	int Level;
	//��ͼid
	int MapId;
	//1����Ҫ����
	int Score1;
	//2����Ҫ����
	int Score2;
	//3����Ҫ����
	int Score3;
	//���ɼ�����������
	int RemoveCount;
	//���ֵ�������
	unsigned char BallIds[64];
	//�������ʼ�����ּ���
	unsigned char SpecialBallIds[128];
	//������������
	unsigned char RewardsProps[32];
	//����ʹ�õĵ���
	unsigned char CanUseProps[32];
	//��������ּ���
	int MagicBallRate;
	//��ʼ���ָ���
	int InitCount;
	//���ֵ��߳��ּ���
	unsigned char PropsRate[32];
};

#pragma pack(pop)

#endif
