#include "HawkRefCounter.h"

namespace Hawk
{
	HawkRefCounter::HawkRefCounter() : m_iRef(1)
	{
	}

	HawkRefCounter::~HawkRefCounter()
	{
	}

	Int32 HawkRefCounter::AddRef()
	{
        return __sync_add_and_fetch(&m_iRef, 1);
	}

	Int32 HawkRefCounter::DecRef()
	{ 
        return __sync_sub_and_fetch(&m_iRef, 1);
	}

	Int32 HawkRefCounter::GetRef() const
	{ 
		return m_iRef;
	}

	void HawkRefCounter::Release()
	{
		//减少引用计数
		Long iRef = DecRef();

		//删除对象
		if (iRef == 0)
		{
			delete this;
		}
	}

	void* HawkRefCounter::operator new(Size_t iSize)
	{
		return HawkMalloc(iSize);
	}

	void HawkRefCounter::operator delete(void* pData, Size_t iSize)
	{
		HawkFree(pData);
	}
}
