#ifndef CFG_PROPSDATA_H
#define CFG_PROPSDATA_H

#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#include "HawkUtil.h"
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "../../hawksdk/HawkUtil.h"
#endif

#pragma pack(push)
#pragma pack(1)

struct propsData
{
	//����id
	int PropsId;
	//��������
	int PropsType;
	//ѡ��ʱ�����¼�
	unsigned char OnPick[1024];
	//����ʱ�����¼�
	unsigned char OnPut[1024];
	//��ʾʱ�����¼�
	unsigned char OnShow[1024];
	//��Χ������ʱ�����¼�
	unsigned char OnAroundRemove[1024];
};

#pragma pack(pop)

#endif
