#include "StdAfx.h"
#include "BsDefClsFrameInf.h"

#include "BsDefStruct.h"

CFrameInf::CFrameInf( const char *p_data, int i_dataLen, PVOID p_dataHeader)
{
	m_pData = new unsigned char[i_dataLen];
	memcpy(m_pData, p_data, i_dataLen);
	m_dataLen = i_dataLen;
	m_pDataHeader = p_dataHeader;


	m_alarmType = E_FRAME_WARN_TYPE_NORMAL;
	m_alarmCls = E_FRAME_WARN_CLS_0;
	m_alarmMessage = "";
	m_alarmValue = 0.0;

	m_filePath = "";
}

CFrameInf::~CFrameInf()
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}
}
