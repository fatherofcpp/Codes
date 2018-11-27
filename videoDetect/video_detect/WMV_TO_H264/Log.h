/************************************************************************ 
* �ļ�����    log.h 
* �ļ�������  ���ڼ�¼��־����CLog 
* �汾�ţ�    2.0 
************************************************************************/ 
#ifndef _LOG_H
#define _LOG_H

class CLog
{
public:
	CLog();
	~CLog();

public:
	void	InitLog(LPCTSTR lpszLogPath);		
	void	Add(const char* fmt, ...);		//������֣������͸�printfһ��
	
protected:
	enum {BUFSIZE = 3000};  //����������
	char		m_tBuf[BUFSIZE];
	
	CString	 m_strLogPath;
	CRITICAL_SECTION  m_crit;  //����һ���ٽ���
};

#endif