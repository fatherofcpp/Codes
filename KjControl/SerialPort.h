/*
**	FILENAME			CSerialPort.h
**
**	PURPOSE				This class can read, write and watch one serial port.
**						It sends messages to its owner when something happends on the port
**						The class creates a thread for reading and writing so the main
**						program is not blocked.
**
**	CREATION DATE		15-09-1997
**	LAST MODIFICATION	12-11-1997
**
**	AUTHOR				Remon Spekreijse
**
**
************************************************************************************
**  author: mrlong date:2007-12-25
**
**  �Ľ�
**    1) ����ClosePort
**    2) ���� writetoProt() ��������
**    3) ���� SendData �� RecvData ����
** 
************************************************************************************
**  author: itas109  date:2014-01-10
**  Blog��blog.csdn.net/itas109
**
**  �Ľ�
**    1) ���COM10���϶˿��޷���ʾ������
**    2) ��չ��ѡ��˿ڣ����ֵMaxSerialPortNum�����Զ���
**    3) ���QueryKey()��Hkey2ComboBox���������������Զ���ѯ��ǰ��Ч�Ĵ��ںš�
** 
*/

#ifndef __SERIALPORT_H__
#define __SERIALPORT_H__

#define WM_COMM_BREAK_DETECTED		WM_USER+1	// A break was detected on input.
#define WM_COMM_CTS_DETECTED		WM_USER+2	// The CTS (clear-to-send) signal changed state. 
#define WM_COMM_DSR_DETECTED		WM_USER+3	// The DSR (data-set-ready) signal changed state. 
#define WM_COMM_ERR_DETECTED		WM_USER+4	// A line-status error occurred. Line-status errors are CE_FRAME, CE_OVERRUN, and CE_RXPARITY. 
#define WM_COMM_RING_DETECTED		WM_USER+5	// A ring indicator was detected. 
#define WM_COMM_RLSD_DETECTED		WM_USER+6	// The RLSD (receive-line-signal-detect) signal changed state. 
#define WM_COMM_RXCHAR				WM_USER+7	// A character was received and placed in the input buffer. 
#define WM_COMM_RXFLAG_DETECTED		WM_USER+8	// The event character was received and placed in the input buffer.  
#define WM_COMM_TXEMPTY_DETECTED	WM_USER+9	// The last character in the output buffer was sent.  

#define MaxSerialPortNum 20   ///��Ч�Ĵ����ܸ��������Ǵ��ڵĺ� //add by itas109 2014-01-09

typedef struct _tagSerialInfo
{
    UINT port;			//�˿ں�
    UINT baud;			//������
    char parity;		//��żУ��
    UINT databits;		//����λ
    UINT stopbits;		//ֹͣλ
} SERIAL_INFO;

void SendError(int);
//typedef void (CALLBACK *SERIAL_CALLBACK_FUN)(LPVOID context, unsigned char* data, int Len, int port);
typedef void (CALLBACK *SERIAL_CALLBACK_FUN)(LPVOID context, char* data, int Len, int port);
class CSerialPort
{														 
public:

	// contruction and destruction
	CSerialPort();
	virtual		~CSerialPort();

	// port initialisation
	///���ڳ�ʼ��
	BOOL		InitPort(CWnd* pPortOwner, //���ھ��
		                 UINT portnr = 1, //�˿ں�
		                 UINT baud = 19200, //������
		                 char parity = 'N', //��żУ��
		                 UINT databits = 8, //����λ
		                 UINT stopbits = 1, //ֹͣλ
		                 DWORD dwCommEvents = EV_RXCHAR, //��Ϣ����
		                 UINT writebuffersize = 1024//д����
						 );

	BOOL IsOpen() const { return m_hComm != INVALID_HANDLE_VALUE; };
	// start/stop comm watching
	///���ƴ��ڼ����߳�
	BOOL		 StartMonitoring();//��ʼ����
	BOOL		 RestartMonitoring();//���¼���
	BOOL		 StopMonitoring();//ֹͣ����
    void         SetCallback(SERIAL_CALLBACK_FUN func, void *context);

	DWORD		 GetWriteBufferSize();///��ȡд�����С
	DWORD		 GetCommEvents();///��ȡ�¼�
	DCB			 GetDCB();///��ȡDCB

	COMMTIMEOUTS GetCommTimeOuts();//
	BOOL         SetCommTimeOuts(COMMTIMEOUTS * lpTimeOuts);

	///д���ݵ�����
	void		WriteToPort(char* string);
	void		WriteToPort(char* string,int n);//add by mrlong 2007-12-25
	void		WriteToPort(LPCTSTR string);//add by mrlong 2007-12-25
	void		WriteToPort(LPCTSTR string,int n);//add by mrlong 2007-12-2
	void        WriteToPort(BYTE* Buffer, int n);// add by mrlong
	void		ClosePort();//�رմ��� // add by mrlong 2007-12-25

	BOOL        RecvData(LPTSTR lpszData, const int nSize);//���ڷ��ͺ��� by mrlong 2008-2-15
	void        SendData(LPCTSTR lpszData, const int nLength);//���ڽ��պ��� by mrlong 2008-2-15

	void QueryKey(HKEY hKey);///��ѯע���Ĵ��ںţ���ֵ����������
	void Hkey2ComboBox(CComboBox& m_PortNO);///��QueryKey��ѯ���Ĵ��ں���ӵ�CComboBox�ؼ���
	void/*DWORD*/ SetAffinity(DWORD CPUNo);
	BOOL SetPriority(int tPriority);
protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);///������
	static UINT	CommThread(LPVOID pParam);///�̺߳���
	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);///�����ַ�
	static void	WriteChar(CSerialPort* port);///д�ַ�
	

	// thread
	CWinThread*			m_Thread;///thread�����߳�
	BOOL                m_bIsSuspened;///thread�����߳��Ƿ����
	
	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;///�ٽ���Դ
	BOOL				m_bThreadAlive;///�����߳����б�־
	// handles
	HANDLE				m_hWriteEvent;
	HANDLE				m_hComm;///���ھ��
	HANDLE				m_hShutdownEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed.
	///�¼����飬����һ��д�¼��������¼����ر��¼�
	///һ��Ԫ������һ���¼����������¼��̴߳���˿ڡ�
	///д�¼��ͽ����ַ��¼�λ��overlapped�ṹ�壨m_ov.hEvent����
	///���˿ڹر�ʱ����һ��ͨ�õĹرա�
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;///�첽I/O
	COMMTIMEOUTS		m_CommTimeouts;///��ʱ����
	DCB					m_dcb;///�豸���ƿ�

	// owner window
	CWnd*				m_pOwner;

	// misc
	UINT				m_nPortNr;
	char*				m_szWriteBuffer;///д������
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;///д�����С

	int m_nWriteSize; //д���ֽ��� //add by mrlong 2007-12-25

    SERIAL_CALLBACK_FUN m_OnCallBackFunc;
    LPVOID              m_pContext;
};

#endif __SERIALPORT_H__


