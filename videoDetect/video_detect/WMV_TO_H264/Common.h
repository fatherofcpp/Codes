#pragma once

#include "stdafx.h"

class CCommon
{
public:
	CCommon(void);
	~CCommon(void);

public:
	virtual int GetInforFromIni(void *  iniPath){return 0;};
	virtual int SetDefaultParams(void * pParams){return 0;};
	virtual int ParseRequest(char * pXmlBuf,CString *str){return 0;};
	virtual int SetParams(void *pStream,void * pParams){return 0;};
	// 启动拉流函数
	virtual int StartPull(void *pStream,void* pParams){return 0;};
	virtual int ClosePull(void *pStream,void * pParams){return 0;};
	virtual CString CheckExit(CString str){return "";};
	virtual CString ChangeChannel(CString sId,int CH,CString str){return "";};

	virtual int InitEncode(void *pParams){return 0;};
	virtual int EncodeFrame(void *pIn,void *pOut){return 0;};
	virtual int CloseEncode(void *pParams){return 0;};
};
