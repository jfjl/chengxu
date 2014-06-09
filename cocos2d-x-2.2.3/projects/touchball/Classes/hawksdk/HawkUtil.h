#ifndef HAWK_UTIL_H
#define HAWK_UTIL_H

#if !defined(UTIL_EXPORT)
#	include "HawkBinCfgFile.h"
#	include "HawkDiskFile.h"
#	include "HawkFile.h"
#	include "HawkMalloc.h"
#	include "HawkMemoryFile.h"
#	include "HawkRefCounter.h"
#	include "HawkScope.h"
#	include "HawkStdHeader.h"
#	include "HawkUtil.h"
#	include "HawkZlib.h"
	using namespace Hawk;
#else
#	include "HawkStdHeader.h"
#endif

namespace Hawk
{
	/************************************************************************/
	/* Ӧ�õײ��ʼ��,����,ֹͣ,�ͷŵ�ͳһ�ӿڷ�װ                          */
	/************************************************************************/
	class UTIL_API HawkUtil
	{
	public:
		//��ʼ��
		static void Init();

		//���ڸ���
		static void Tick(UInt32 iPeriod);

		//ֹͣ����
		static void Stop();

		//�ͷ���Դ
		static void Release();
	};
};
#endif
