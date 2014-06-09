#include "HawkBinCfgFile.h"
#include "HawkDiskFile.h"
#include "HawkScope.h"
#include "HawkZlib.h"

#undef get16bits
#if (defined(__GNUC__) && defined(__i386__)) || defined(__WATCOMC__) || defined(_MSC_VER) || defined (__BORLANDC__) || defined (__TURBOC__)
#	define get16bits(d) (*((const UInt16 *) (d)))
#endif

#if !defined (get16bits)
#	define get16bits(d) ((((const UInt8 *)(d))[1] << 8)+((const UInt8 *)(d))[0])
#endif

namespace Hawk
{
	HawkBinCfgFile::HawkBinCfgFile()
	{
		m_pFile = 0;
	}

	HawkBinCfgFile::~HawkBinCfgFile()
	{
		HAWK_RELEASE(m_pFile);
	}
    
	UInt32 HawkBinCfgFile::CalcHash(const Char * pData, Size_t iSize)
	{
		if (!pData) return 0;
        
		if (!iSize)
			iSize = strlen(pData);
        
		if (!iSize) return 0;
		
		UInt32 iHash = (UInt32)iSize;
		UInt32 iTmp  = 0;
		Int32  iRem  = 0;
		
		iRem = iSize & 3;
		iSize >>= 2;
        
		for (;iSize > 0; iSize--)
		{
			iHash += get16bits(pData);
			iTmp  = (get16bits(pData+2) << 11) ^ iHash;
			iHash = (iHash << 16) ^ iTmp;
			pData += 2*sizeof (UInt16);
			iHash  += iHash >> 11;
		}
        
		switch (iRem)
		{
            case 3:
                iHash += get16bits(pData);
                iHash ^= iHash << 16;
                iHash ^= pData[sizeof (UInt16)] << 18;
                iHash += iHash >> 11;
                break;
            case 2:
                iHash += get16bits (pData);
                iHash ^= iHash << 11;
                iHash += iHash >> 17;
                break;
            case 1:
                iHash += *pData;
                iHash ^= iHash << 10;
                iHash += iHash >> 1;
		}
        
		iHash ^= iHash << 3;
		iHash += iHash >> 5;
		iHash ^= iHash << 4;
		iHash += iHash >> 17;
		iHash ^= iHash << 25;
		iHash += iHash >> 6;
        
		return iHash;
	}

	Bool HawkBinCfgFile::LoadCfgData(const AString& sFile)
	{
		HawkDiskFile src_file;
		if (src_file.Open(sFile))
		{
			//长度和CRC获取
			Int64 iSize	  = src_file.GetFileSize();
			Int32 iUnSize = 0;
			UInt32 iCrc	  = 0;

			if(src_file.Read(&iUnSize,sizeof(iUnSize)) != sizeof(iUnSize))
				return false;

			Int64 iComSize = iSize - (Int64)sizeof(iUnSize);
			if(src_file.Read(&iCrc,sizeof(iCrc)) != sizeof(iCrc))
				return false;

			iComSize -= (Int64)sizeof(iCrc);

			Char* pComData = new Char[(Size_t)iComSize];
			HawkScope::DataArrayPtr comdata_scope(pComData);
			memset(pComData, 0, (Size_t)iComSize);
			if (src_file.Read(pComData,iComSize) != iComSize)
				return false;

			if (iCrc != CalcHash(pComData, (Size_t)iComSize))
				return false;

			//做位反运算
			for (Int64 i=0;i<iComSize;i++)
				pComData[i] = (~pComData[i]);

			Char* pSrcData = new Char[iUnSize];
			HawkScope::DataArrayPtr srcdata_scope(pSrcData);
			memset(pSrcData,0,iUnSize);

			ULong iSrcSize = (ULong)iUnSize;
			if (!HawkZip::UnCompress(pSrcData, iSrcSize, pComData, (ULong)iComSize))
				return false;

			HAWK_RELEASE(m_pFile);
			m_pFile = new HawkMemoryFile();
			if (!m_pFile->Open(pSrcData, iSrcSize, false))
				return false;

			return true;
		}
		return false;
	}

	Bool HawkBinCfgFile::LoadFromMem(const void* pData, UInt32 iSize)
	{
		const UChar* pDataPtr = (const UChar*)pData;
		if (pData && iSize)
		{		
			//长度和CRC获取
			Int32 iUnSize  = *((Int32*)pDataPtr);
			pDataPtr	   += sizeof(Int32);
			UInt32 iCrc	   = *((UInt32*)pDataPtr);
			pDataPtr	   += sizeof(UInt32);
			Int64 iComSize = iSize - (Int64)(sizeof(iUnSize) + sizeof(iCrc));
			if (iCrc != CalcHash((const Char *)pDataPtr, (Size_t)iComSize))
				return false;

			//拷贝数据
			Char* pComData  = new Char[(Size_t)iComSize];
			HawkScope::DataArrayPtr comdata_scope(pComData);
			memcpy(pComData, pDataPtr, (Size_t)iComSize);

			//做位反运算
			for (Int64 i=0;i<iComSize;i++)
				pComData[i] = (~pComData[i]);

			Char* pSrcData = new Char[iUnSize];
			HawkScope::DataArrayPtr srcdata_scope(pSrcData);
			memset(pSrcData,0,iUnSize);

			ULong iSrcSize = (ULong)iUnSize;
			if (!HawkZip::UnCompress(pSrcData, iSrcSize, pComData, (ULong)iComSize))
				return false;

			HAWK_RELEASE(m_pFile);
			m_pFile = new HawkMemoryFile();
			if (!m_pFile->Open(pSrcData, iSrcSize, false))
				return false;

			return true;
		}
		return false;
	}

	Int64 HawkBinCfgFile::Read(void* pData,Int64 iSize)
	{
		if (m_pFile)
			return m_pFile->Read(pData, iSize);

		return 0;
	}

	Int64 HawkBinCfgFile::Seek(Int64 iOffset,HawkFile::SeekPos ePos)
	{
		if (m_pFile)
			return m_pFile->Seek(iOffset,ePos);

		return 0;
	}
}
