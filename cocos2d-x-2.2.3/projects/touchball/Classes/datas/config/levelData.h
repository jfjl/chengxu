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
	//���ֵ��߳���λ��
	unsigned char PropsPos[128];
	//���Ʋ���
	int MaxStep;
};

#pragma pack(pop)

#endif
