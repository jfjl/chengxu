#ifndef HAWK_MALLOC_H
#define HAWK_MALLOC_H

#include "HawkStdHeader.h"

namespace Hawk
{
	/************************************************************************/
	/* Malloc��ز�����װ													*/
	/************************************************************************/
	//�ڴ����
	void*  UTIL_API HawkMalloc(Size_t iSize);

	//�ڴ����
	void*  UTIL_API HawkCalloc(Size_t iNum, Size_t iSize);

	//�ڴ��ط���
	void*  UTIL_API HawkRealloc(void* pMem, Size_t iSize);

	//�ڴ��ͷ�
	void   UTIL_API HawkFree(void* pMem);
}

#endif
