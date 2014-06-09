#include "HawkMalloc.h"

#if defined(_DEBUG) && defined(PLATFORM_WINDOWS)
#	define NED_MALLOC  0
#else
#	define NED_MALLOC  1
#endif

#if NED_MALLOC
//#	include "nedmalloc.c"
#endif

namespace Hawk
{
	void* HawkMalloc(Size_t iSize)
	{
		if (iSize)
		{
			return malloc(iSize);
		}

		return 0;
	}

	void* HawkCalloc(Size_t iNum, Size_t iSize)
	{
		if (iSize)
		{
			return calloc(iNum, iSize);
		}

		return 0;
	}

	void* HawkRealloc(void* pMem, Size_t iSize)
	{
		if (!pMem)
			return HawkMalloc(iSize);

		if (!iSize)
		{
			HawkFree(pMem);
			return 0;
		}

		return realloc(pMem, iSize);
	}

	void  HawkFree(void* pMem)
	{
		if (pMem)
		{
			free(pMem);
		}
	}
}
