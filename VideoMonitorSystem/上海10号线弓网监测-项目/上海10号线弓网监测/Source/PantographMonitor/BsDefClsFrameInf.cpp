#include "StdAfx.h"
#include "BsDefClsFrameInf.h"
#include "BsDefStruct.h"

CFrameInf::CFrameInf( const char *p_data, int i_dataLen, PVOID p_dataHeader)
{
	m_pData = new unsigned char[i_dataLen];
	memcpy(m_pData, p_data, i_dataLen);
	m_dataLen = i_dataLen;
	m_pDataHeader = p_dataHeader;

	m_pAnalyzeResArr = new AnalyzeResArr;

	m_filePath = "";
	m_isTempSave = false;
	m_isColor = false;
	m_isTempMapSave = false;
	m_isTempAlarm = false;

	m_tempLow = 0;
	m_tempHigh = 0;
	m_tempAvarage = 0;
	m_tempHignLeftUpX = 0;
	m_tempHignLeftUpY = 0;
}

CFrameInf::~CFrameInf()
{
	if (m_pData)
	{
		delete[] m_pData;
		m_pData = NULL;
	}


	if (m_pAnalyzeResArr)
	{
		delete m_pAnalyzeResArr;
	}
}
