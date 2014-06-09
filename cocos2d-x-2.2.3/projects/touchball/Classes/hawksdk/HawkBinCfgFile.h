#ifndef HAWK_BINCFGFILE_H
#define HAWK_BINCFGFILE_H

#include "HawkMemoryFile.h"

namespace Hawk
{
	/************************************************************************/
	/* 压缩配置文件读取类                                                   */
	/************************************************************************/
	class UTIL_API HawkBinCfgFile : public HawkRefCounter
	{
	public:
		//构造
		HawkBinCfgFile();

		//析构
		virtual ~HawkBinCfgFile();

	public:
		//加载配置文件
		virtual Bool    LoadCfgData(const AString& sFile);

		//加载配置文件
		virtual Bool    LoadFromMem(const void* pData, UInt32 iSize);

	public:
		//读取数据
		virtual Int64   Read(void* pData, Int64 iSize);

		//偏移字节
		virtual Int64   Seek(Int64 iOffset, HawkFile::SeekPos ePos = HawkFile::POS_BEGIN);

		UInt32  CalcHash(const Char * pData, Size_t iSize);
	protected:
		//文件句柄
		HawkMemoryFile* m_pFile;
	};
}
#endif
