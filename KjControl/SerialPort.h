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
**  改进
**    1) 增加ClosePort
**    2) 增加 writetoProt() 两个方法
**    3) 增加 SendData 与 RecvData 方法
** 
************************************************************************************
**  author: itas109  date:2014-01-10
**  Blog：blog.csdn.net/itas109
**
**  改进
**    1) 解决COM10以上端口无法显示的问题
**    2) 扩展可选择端口，最大值MaxSerialPortNum可以自定义
**    3) 添加QueryKey()和Hkey2ComboBox两个方法，用于自动查询当前有效的串口号。
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

#define MaxSerialPortNum 20   ///有效的串口总个数，不是串口的号 //add by itas109 2014-01-09

typedef struct _tagSerialInfo
{
    UINT port;			//端口号
    UINT baud;			//波特率
    char parity;		//奇偶校验
    UINT databits;		//数据位
    UINT stopbits;		//停止位
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
	///串口初始化
	BOOL		InitPort(CWnd* pPortOwner, //串口句柄
		                 UINT portnr = 1, //端口号
		                 UINT baud = 19200, //波特率
		                 char parity = 'N', //奇偶校验
		                 UINT databits = 8, //数据位
		                 UINT stopbits = 1, //停止位
		                 DWORD dwCommEvents = EV_RXCHAR, //消息类型
		                 UINT writebuffersize = 1024//写缓存
						 );

	BOOL IsOpen() const { return m_hComm != INVALID_HANDLE_VALUE; };
	// start/stop comm watching
	///控制串口监视线程
	BOOL		 StartMonitoring();//开始监听
	BOOL		 RestartMonitoring();//重新监听
	BOOL		 StopMonitoring();//停止监听
    void         SetCallback(SERIAL_CALLBACK_FUN func, void *context);

	DWORD		 GetWriteBufferSize();///获取写缓冲大小
	DWORD		 GetCommEvents();///获取事件
	DCB			 GetDCB();///获取DCB

	COMMTIMEOUTS GetCommTimeOuts();//
	BOOL         SetCommTimeOuts(COMMTIMEOUTS * lpTimeOuts);

	///写数据到串口
	void		WriteToPort(char* string);
	void		WriteToPort(char* string,int n);//add by mrlong 2007-12-25
	void		WriteToPort(LPCTSTR string);//add by mrlong 2007-12-25
	void		WriteToPort(LPCTSTR string,int n);//add by mrlong 2007-12-2
	void        WriteToPort(BYTE* Buffer, int n);// add by mrlong
	void		ClosePort();//关闭串口 // add by mrlong 2007-12-25

	BOOL        RecvData(LPTSTR lpszData, const int nSize);//串口发送函数 by mrlong 2008-2-15
	void        SendData(LPCTSTR lpszData, const int nLength);//串口接收函数 by mrlong 2008-2-15

	void QueryKey(HKEY hKey);///查询注册表的串口号，将值存于数组中
	void Hkey2ComboBox(CComboBox& m_PortNO);///将QueryKey查询到的串口号添加到CComboBox控件中
	void/*DWORD*/ SetAffinity(DWORD CPUNo);
	BOOL SetPriority(int tPriority);
protected:
	// protected memberfunctions
	void		ProcessErrorMessage(char* ErrorText);///错误处理
	static UINT	CommThread(LPVOID pParam);///线程函数
	static void	ReceiveChar(CSerialPort* port, COMSTAT comstat);///接收字符
	static void	WriteChar(CSerialPort* port);///写字符
	

	// thread
	CWinThread*			m_Thread;///thread监视线程
	BOOL                m_bIsSuspened;///thread监视线程是否挂起
	
	// synchronisation objects
	CRITICAL_SECTION	m_csCommunicationSync;///临界资源
	BOOL				m_bThreadAlive;///监视线程运行标志
	// handles
	HANDLE				m_hWriteEvent;
	HANDLE				m_hComm;///串口句柄
	HANDLE				m_hShutdownEvent;

	// Event array. 
	// One element is used for each event. There are two event handles for each port.
	// A Write event and a receive character event which is located in the overlapped structure (m_ov.hEvent).
	// There is a general shutdown when the port is closed.
	///事件数组，包括一个写事件，接收事件，关闭事件
	///一个元素用于一个事件。有两个事件线程处理端口。
	///写事件和接收字符事件位于overlapped结构体（m_ov.hEvent）中
	///当端口关闭时，有一个通用的关闭。
	HANDLE				m_hEventArray[3];

	// structures
	OVERLAPPED			m_ov;///异步I/O
	COMMTIMEOUTS		m_CommTimeouts;///超时设置
	DCB					m_dcb;///设备控制块

	// owner window
	CWnd*				m_pOwner;

	// misc
	UINT				m_nPortNr;
	char*				m_szWriteBuffer;///写缓冲区
	DWORD				m_dwCommEvents;
	DWORD				m_nWriteBufferSize;///写缓冲大小

	int m_nWriteSize; //写入字节数 //add by mrlong 2007-12-25

    SERIAL_CALLBACK_FUN m_OnCallBackFunc;
    LPVOID              m_pContext;
};

#endif __SERIALPORT_H__


