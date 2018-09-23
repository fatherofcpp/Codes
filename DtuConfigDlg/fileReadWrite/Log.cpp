#include "stdafx.h"
#include "log.h"
#include <stdio.h>
#define log_print TRACE

CLog gLog;
bool isLog = true;

CLog::CLog()  //���캯����������־�ļ���Ĭ��·��
{
	::InitializeCriticalSection(&m_crit);   //��ʼ���ٽ���
}


CLog::~CLog()
{
	::DeleteCriticalSection(&m_crit);    //�ͷ����ٽ���
}


/*================================================================ 
* ��������    InitLog
* ������      LPCTST lpszLogPath
* ��������:   ��ʼ����־(������־�ļ���·��)
* ����ֵ��    void
================================================================*/ 
void CLog::InitLog(LPCTSTR lpszLogPath)   
{	
	m_strLogPath=lpszLogPath;
}

void CLog::Add(const char* fmt, ...)
{
	if (!isLog)
	{
		return;
	}

	if (m_strLogPath.IsEmpty())
		return ;

	if (!AfxIsValidString(fmt, -1))
		return ;
	/*-----------------------�����ٽ���(д�ļ�)------------------------------------*/	
	::EnterCriticalSection(&m_crit);   
	try      
	{
		va_list argptr;          //�����ַ����ĸ�ʽ
		va_start(argptr, fmt);
		_vsnprintf(m_tBuf, BUFSIZE, fmt, argptr);
		va_end(argptr);
	}
	catch (...)
	{
		m_tBuf[0] = 0;
	}

	FILE *fp = fopen(m_strLogPath, "a"); //����ӵķ�ʽ������ļ�
	if (fp)
	{
		fprintf(fp,"%s:  ", AfxGetApp()->m_pszExeName);  //���뵱ǰ������

		CTime ct ;                          //���뵱ǰʱ��
		ct = CTime::GetCurrentTime();
		fprintf(fp,"%s : ",ct.Format("%m/%d/%Y %H:%M:%S"));
		fprintf(fp, "%s\n", m_tBuf);	
		log_print("%s\n",m_tBuf);

		fseek(fp,0L,SEEK_END);
		int filelen = ftell(fp);
		fclose(fp);		
		if(filelen>8*1024*1024)
		{
			CString tmpLog;
			tmpLog.Format("%s.log",m_strLogPath.GetBuffer(m_strLogPath.GetLength()));
			m_strLogPath.ReleaseBuffer();
			remove(tmpLog);
			rename(m_strLogPath,tmpLog);//��Ҫ��ɾ��ԭ�ļ�
		}

	}	
	::LeaveCriticalSection(&m_crit);  
	/*-------------------�˳��ٽ���----------------------------------------*/	
}


