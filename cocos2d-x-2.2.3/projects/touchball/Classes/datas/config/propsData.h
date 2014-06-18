#ifndef CFG_PROPSDATA_H
#define CFG_PROPSDATA_H

/************************************************************************/
/* Author: daijunhua                                                    */
/************************************************************************/
#include "HawkUtil.h"

#pragma pack(push)
#pragma pack(1)

struct propsData
{
	//����id
	int PropsId;
	//��������
	int PropsType;
	//����ʱ�����¼�
	unsigned char OnPut[1024];
	//��ʾʱ�����¼�
	unsigned char OnShow[1024];
	//��Χ������ʱ�����¼�
	unsigned char OnAroundRemove[1024];
};

#pragma pack(pop)

#endif
