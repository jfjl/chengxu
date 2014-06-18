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
	//道具id
	int PropsId;
	//道具类型
	int PropsType;
	//放置时发生事件
	unsigned char OnPut[1024];
	//显示时发生事件
	unsigned char OnShow[1024];
	//周围的球消时发生事件
	unsigned char OnAroundRemove[1024];
};

#pragma pack(pop)

#endif
