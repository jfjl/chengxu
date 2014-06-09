#include "HawkMemoryFile.h"
#include "HawkDiskFile.h"

namespace Hawk
{
	HawkMemoryFile::HawkMemoryFile()
	{
		m_iFilePos = 0;
		m_pData    = 0;
		m_bExtra   = false;
	}

	HawkMemoryFile::~HawkMemoryFile()
	{
		Close();
	}

	Bool HawkMemoryFile::Open(Int64 iSize, OpenType eOpen)
	{
		Close();
	
		m_eOpenType = eOpen;
		m_iFileSize = iSize;
		m_bExtra    = false;

		if(iSize > 0)
		{
			m_pData = (Char*)malloc((Size_t)m_iFileSize);
			memset(m_pData, 0, (Size_t)m_iFileSize);
		}

		return true;
	}

	Bool HawkMemoryFile::Open(void* pData, Int64 iSize, Bool bExtra, OpenType eOpen)
	{
		Close();

		m_eOpenType = eOpen;
		m_iFileSize = iSize;
		m_bExtra	= bExtra;

		if (!m_bExtra)
		{
			m_pData = (Char*)malloc((Size_t)m_iFileSize);
			memset(m_pData, 0, (Size_t)m_iFileSize);
			memcpy(m_pData, pData, (Size_t)m_iFileSize);
		}
		else
		{
			m_pData	= pData;
		}		

		return true;
	}

	Bool HawkMemoryFile::Open(const AString& sFile)
	{
		Close();

		m_eOpenType = OPEN_READ;
		m_bExtra    = false;

		HawkDiskFile df;
		if (df.Open(sFile))
		{
			m_iFileSize = df.GetFileSize();

			m_pData = (Char*)malloc((Size_t)m_iFileSize);
			memset(m_pData, 0, (Size_t)m_iFileSize);

			df.Read(m_pData, m_iFileSize);
			return true;
		}
		return false;
	}

	Bool HawkMemoryFile::Close()
	{
		m_iFilePos  = 0;
		m_iFileSize = 0;

		if (!m_bExtra)
		{
			free(m_pData);
			m_pData = 0;
		}
		else
		{
			m_pData = 0;
		}

		return true;
	}
    

	Int64 HawkMemoryFile::Read(void* pData, Int64 iSize)
	{
		if (!pData || iSize <= 0 || (m_eOpenType != OPEN_READ && m_eOpenType != OPEN_RW))
			return 0;
		
		iSize = m_iFileSize - m_iFilePos < iSize ? m_iFileSize - m_iFilePos : iSize;
		
		memcpy(pData, (Char*)m_pData + m_iFilePos, (Size_t)iSize);

		m_iFilePos += iSize;

		return iSize;
	}

	Int64 HawkMemoryFile::Write(const void* pData, Int64 iSize, Bool bFlush)
	{
		if (!pData || iSize <= 0 || (m_eOpenType != OPEN_WRITE && m_eOpenType != OPEN_RW && m_eOpenType != OPEN_APPEND) )
			return 0;

		//容量不足
		if (m_iFilePos + iSize > m_iFileSize) 
		{
			//扩大一倍
			while (m_iFileSize < m_iFilePos + iSize)
				m_iFileSize *= 2;

			//重新分配
			m_pData = realloc(m_pData, (Size_t)m_iFileSize);
			memset((Char*)m_pData + (Size_t)m_iFilePos, 0, (Size_t)(m_iFileSize - m_iFilePos));
		}

		memcpy((Char*)m_pData + m_iFilePos, pData, (Size_t)iSize);
		m_iFilePos += iSize;

		return iSize;
	}

	Int64 HawkMemoryFile::Tell()
	{
		return m_iFilePos;
	}

	Int64 HawkMemoryFile::Seek(Int64 iOffset, SeekPos ePos)
	{
		Int64 iRealPos = m_iFilePos;
		Int64 iOldPos  = m_iFilePos;
		if (ePos == POS_BEGIN)
		{
			iRealPos = iOffset;
		}
		else if (ePos == POS_CURRENT)
		{
			iRealPos = m_iFilePos + iOffset;
		}
		else
		{
			iRealPos = m_iFileSize + iOffset;
		}

		if(iRealPos > m_iFileSize)
		{
			if((m_eOpenType == OPEN_WRITE || m_eOpenType == OPEN_RW || m_eOpenType == OPEN_APPEND))
			{				
				m_pData = realloc(m_pData, (Size_t)iRealPos);
				memset((Char*)m_pData + (Size_t)m_iFilePos, 0, (Size_t)(iRealPos - m_iFilePos));
				m_iFileSize = iRealPos;
				m_iFilePos  = iRealPos;
			}
			else
			{
				m_iFilePos = m_iFileSize;
			}
		}
		else
		{
			m_iFilePos = iRealPos;
		}

		return m_iFilePos - iOldPos;
	}

	Bool HawkMemoryFile::Chsize(Int64 iSize)
	{		
		if((m_eOpenType == OPEN_WRITE || m_eOpenType == OPEN_RW || m_eOpenType == OPEN_APPEND))
		{
			if (m_iFileSize < iSize)
			{
				m_pData = realloc(m_pData, (Size_t)iSize);
				memset((Char*)m_pData, 0, (Size_t)iSize);
			}

			m_iFileSize = iSize;
			m_iFilePos  = 0;

			return true;
		}
		return false;
	}

	Bool HawkMemoryFile::SaveToDisk(const AString& sFile)
	{
		HawkDiskFile df;
		if (m_pData && df.Open(sFile, OPEN_WRITE))
		{
			if (m_iFilePos > 0)
			{
				Int64 iSize  = df.Write(m_pData, m_iFilePos);
				return iSize == m_iFilePos;
			}
			return true;
		}
		return false;
	}
}
