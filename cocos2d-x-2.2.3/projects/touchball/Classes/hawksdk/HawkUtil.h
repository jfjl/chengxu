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
	/* 应用底层初始化,更新,停止,释放的统一接口封装                          */
	/************************************************************************/
	class UTIL_API HawkUtil
	{
	public:
		//初始化
		static void Init();

		//周期更新
		static void Tick(UInt32 iPeriod);

		//停止运行
		static void Stop();

		//释放资源
		static void Release();
	};
};
#endif
