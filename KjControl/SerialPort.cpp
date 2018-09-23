/*
**	FILENAME			CSerialPort.cpp
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
*/


#include "stdafx.h"
#include "SerialPort.h"
//#include "..\console.h"
//#include "beidoudll.h"

#include <assert.h>
#pragma warning(disable : 4996)
int m_nComArray[20];

extern BOOL Port_ERROR;

enum EResponseState
{
    eSucceed = 0,//�ɹ�����0
    eFailed  = 1,//ʧ�ܷ���1
    ePortOpeFailed,//���ڴ�ʧ��
    ePortDisconnect,//��������ʧ��
};

//
// Constructor
//
CSerialPort::CSerialPort()///���캯��
{
	m_hComm = NULL;
	// initialize overlapped structure members to zero
	///��ʼ���첽�ṹ��
	m_ov.Offset = 0;
	m_ov.OffsetHigh = 0;

	// create events
	m_ov.hEvent = NULL;
	m_hWriteEvent = NULL;
	m_hShutdownEvent = NULL;

	m_szWriteBuffer = NULL;
	m_bThreadAlive = FALSE;

	m_nWriteSize=1;///
    m_bIsSuspened = FALSE;///
    m_OnCallBackFunc = NULL;
    m_pContext = NULL;
}

//
// Delete dynamic memory
//
CSerialPort::~CSerialPort()///��������
{
	do
	{
		SetEvent(m_hShutdownEvent);
	} while (m_bThreadAlive);

	
	// if the port is still opened: close it 
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}
	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		CloseHandle( m_hShutdownEvent); 
	if(m_ov.hEvent!=NULL)
		CloseHandle( m_ov.hEvent ); 
	if(m_hWriteEvent!=NULL)
		CloseHandle( m_hWriteEvent ); 

	TRACE("Thread ended\n");
	delete [] m_szWriteBuffer;
}

//
// Initialize the port. This can be port 1 to MaxSerialPortNum.
///��ʼ�����ڡ�ֻ����1-MaxSerialPortNum
//
BOOL CSerialPort::InitPort(CWnd* pPortOwner,	// the owner (CWnd) of the port (receives message)
						   UINT  portnr,		// portnumber (1..MaxSerialPortNum)
						   UINT  baud,			// baudrate
						   char  parity,		// parity 
						   UINT  databits,		// databits 
						   UINT  stopbits,		// stopbits 
						   DWORD dwCommEvents,	// EV_RXCHAR, EV_CTS etc
						   UINT  writebuffersize)	// size to the writebuffer
{
	assert(portnr > 0 && portnr < MaxSerialPortNum+1);///add by itas109 2014-01-09
	//assert(pPortOwner != NULL);

	// if the thread is alive: Kill
	///����̴߳��ڣ���ص�����
	if (m_bThreadAlive)
	{
		do
		{
			SetEvent(m_hShutdownEvent);
		} while (m_bThreadAlive);
		TRACE("Thread ended\n");
	}

	// create events
	if (m_ov.hEvent != NULL)
		ResetEvent(m_ov.hEvent);
	else
		m_ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (m_hWriteEvent != NULL)
		ResetEvent(m_hWriteEvent);
	else
		m_hWriteEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	if (m_hShutdownEvent != NULL)
		ResetEvent(m_hShutdownEvent);
	else
		m_hShutdownEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	// initialize the event objects
	///�¼������ʼ�����趨���ȼ���
	m_hEventArray[0] = m_hShutdownEvent;	// highest priority
	m_hEventArray[1] = m_ov.hEvent;
	m_hEventArray[2] = m_hWriteEvent;

	// initialize critical section
	///��ʼ���ٽ���Դ
	InitializeCriticalSection(&m_csCommunicationSync);
	
	// set buffersize for writing and save the owner
	//m_pOwner = pPortOwner;

	if (m_szWriteBuffer != NULL)
		delete [] m_szWriteBuffer;
	m_szWriteBuffer = new char[writebuffersize];

	m_nPortNr = portnr;

	m_nWriteBufferSize = writebuffersize;
	m_dwCommEvents = dwCommEvents;

	BOOL bResult = FALSE;
	char *szPort = new char[50];
	char *szBaud = new char[50];

	
	/*
	����̲߳�����ͬ������ʱ��һ������Ҫ��˳����ʵģ�������������ݴ��ң�
	�޷��������ݣ�������������Ϊ���������⣬����Ҫ���뻥���������ÿ
	���̶߳���˳��ط��ʱ�������������Ҫʹ��EnterCriticalSection��
	LeaveCriticalSection������
	*/
	// now it critical!
	///�����ٽ���
	EnterCriticalSection(&m_csCommunicationSync);

	// if the port is already opened: close it
	///�����Ѵ򿪾͹ص�
	if (m_hComm != NULL)
	{
		CloseHandle(m_hComm);
		m_hComm = NULL;
	}

	// prepare port strings
	///���ڲ���
	sprintf(szPort, "\\\\.\\COM%d", portnr);///������ʾCOM10���϶˿�//add by itas109 2014-01-09
	sprintf(szBaud, "baud=%d parity=%c data=%d stop=%d", baud, parity, databits, stopbits);

	// get a handle to the port
	/*
	ͨ�ų�����CreateFile��ָ�������豸����صĲ������ԣ��ٷ���һ�������
	�þ���������ں�����ͨ�Ų��������ᴩ����ͨ�Ź��̴��ڴ򿪺�������
	������ΪĬ��ֵ�����ݾ�����Ҫ��ͨ������GetCommState(hComm,&&dcb)��ȡ
	��ǰ�����豸���ƿ�DCB���ã��޸ĺ�ͨ��SetCommState(hComm,&&dcb)����д
	�롣����ReadFile()��WriteFile()������API����ʵ�ִ��ڶ�д��������Ϊ��
	��ͨ�ŷ�ʽ�������������һ������Ϊָ��OVERLAPPED�ṹ�ķǿ�ָ�룬�ڶ�
	д��������ֵΪFALSE������£�����GetLastError()����������ֵΪERROR_IO_PENDING��
	����I/O�������ң�������ת���̨����ִ�С���ʱ��������WaitForSingleObject()
	���ȴ������źŲ�������ȴ�ʱ��
	*/
	m_hComm = CreateFile(szPort,						// communication port string (COMX)
					     GENERIC_READ | GENERIC_WRITE,	// read/write types
					     0,								// comm devices must be opened with exclusive access
					     NULL,							// no security attributes
					     OPEN_EXISTING,					// comm devices must use OPEN_EXISTING
					     FILE_FLAG_OVERLAPPED,			// Async I/O
					     0);							// template must be 0 for comm devices

	///����ʧ��
	if (m_hComm == INVALID_HANDLE_VALUE)
	{
		// port not found
		delete [] szPort;
		delete [] szBaud;

		return FALSE;
	}

	// set the timeout values
	///���ó�ʱ
	m_CommTimeouts.ReadIntervalTimeout = 1000;
	m_CommTimeouts.ReadTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.ReadTotalTimeoutConstant = 1000;
	m_CommTimeouts.WriteTotalTimeoutMultiplier = 1000;
	m_CommTimeouts.WriteTotalTimeoutConstant = 1000;

	// configure
	///����
	///�ֱ����Windows API���ô��ڲ���
	if (SetCommTimeouts(m_hComm, &m_CommTimeouts))///���ó�ʱ
	{
		/*
		���Զ˿����ݵ���Ӧʱ��Ҫ����ϸ񣬿ɲ����¼�������ʽ��
		�¼�������ʽͨ�������¼�֪ͨ������ϣ�����¼�����ʱ��Windows
		�������¼��ѷ�����֪ͨ������DOS�����µ��жϷ�ʽ�����ơ�Windows
	    ������9�ִ���ͨ���¼����ϳ��õ����������֣�
			EV_RXCHAR:���յ�һ���ֽڣ����������뻺������
			EV_TXEMPTY:����������е����һ���ַ������ͳ�ȥ��
			EV_RXFLAG:���յ��¼��ַ�(DCB�ṹ��EvtChar��Ա)���������뻺����
		����SetCommMask()ָ�������õ��¼���Ӧ�ó���ɵ���WaitCommEvent()���ȴ���
		���ķ�����SetCommMask(hComm,0)��ʹWaitCommEvent()��ֹ
		*/
		if (SetCommMask(m_hComm, dwCommEvents))///����ͨ���¼�
		{
			if (GetCommState(m_hComm, &m_dcb))///��ȡ��ǰDCB����
			{
				m_dcb.EvtChar = 'q';///�����ּ��ַ�
				m_dcb.fRtsControl = RTS_CONTROL_ENABLE;		// set RTS bit high!
				if (BuildCommDCB(szBaud, &m_dcb))///��дDCB�ṹ
				{
					if (SetCommState(m_hComm, &m_dcb))///����DCB
						; // normal operation... continue
					else
						ProcessErrorMessage("SetCommState()");
				}
				else
					ProcessErrorMessage("BuildCommDCB()");
			}
			else
				ProcessErrorMessage("GetCommState()");
		}
		else
			ProcessErrorMessage("SetCommMask()");
	}
	else
		ProcessErrorMessage("SetCommTimeouts()");

	delete [] szPort;
	delete [] szBaud;

	// flush the port
	///��ֹ��д����ս��պͷ���
	PurgeComm(m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// release critical section
	///�ͷ��ٽ���Դ
	LeaveCriticalSection(&m_csCommunicationSync);

	TRACE("Initialisation for communicationport %d completed.\nUse Startmonitor to communicate.\n", portnr);

	return TRUE;
}

//
//  The CommThread Function.
///�̺߳���
///�����̵߳Ĵ������̣�
///��鴮��-->����ѭ��{WaitCommEvent(������ѯ��)ѯ���¼�-->������¼�����-->����Ӧ����(�ر�\��\д)}
//
UINT CSerialPort::CommThread(LPVOID pParam)
{
	// Cast the void pointer passed to the thread back to
	// a pointer of CSerialPort class
	///CSerialPort���ָ��
	CSerialPort *port = (CSerialPort*)pParam;
	
	// Set the status variable in the dialog class to
	// TRUE to indicate the thread is running.
	///TRUE��ʾ�߳���������
	port->m_bThreadAlive = TRUE;	
		
	// Misc. variables
	DWORD BytesTransfered = 0; 
	DWORD Event = 0;
	DWORD CommEvent = 0;
	DWORD dwError = 0;
	COMSTAT comstat;
	BOOL  bResult = TRUE;
		
	// Clear comm buffers at startup
	///��ʼʱ������ڻ���
	if (port->m_hComm)		// check if the port is opened
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

	// begin forever loop.  This loop will run as long as the thread is alive.
	///ֻҪ�̴߳��ھͲ��϶�ȡ����
	//CEditLog::GetInstance()->Output("�˿ڣ�%d �߳���������ʼ������\r\n", port->m_nPortNr);
	for (;;) 
	{ 

		// Make a call to WaitCommEvent().  This call will return immediatly
		// because our port was created as an async port (FILE_FLAG_OVERLAPPED
		// and an m_OverlappedStructerlapped structure specified).  This call will cause the 
		// m_OverlappedStructerlapped element m_OverlappedStruct.hEvent, which is part of the m_hEventArray to 
		// be placed in a non-signeled state if there are no bytes available to be read,
		// or to a signeled state if there are bytes available.  If this event handle 
		// is set to the non-signeled state, it will be set to signeled when a 
		// character arrives at the port.

		// we do this for each port!

		/*
		WaitCommEvent������3������1pOverlapped������һ��OVERLAPPED�ṹ�ı���ָ��
		��Ҳ������NULL������NULLʱ����ʾ�ú�����ͬ���ģ������ʾ�ú������첽�ġ�
		����WaitCommEventʱ������첽��������������ɣ�����������FALSE��ϵͳ��
		WaitCommEvent����ǰ��OVERLAPPED�ṹ��ԱhEvent��Ϊ���ź�״̬���ȵ�����ͨ��
		�¼�ʱ��ϵͳ���������ź�
		*/

		bResult = WaitCommEvent(port->m_hComm, &Event, &port->m_ov);///��ʾ�ú������첽��
		
		if (!bResult)  
		{ 
			// If WaitCommEvent() returns FALSE, process the last error to determin
			// the reason..
			///���WaitCommEvent����ErrorΪFALSE�����ѯ������Ϣ
			switch (dwError = GetLastError()) 
            { 
            case ERROR_INVALID_HANDLE:
                break;
            case ERROR_BAD_COMMAND:
                port->m_bThreadAlive = FALSE;
                SetEvent(port->m_hShutdownEvent);
                //SendError(ePortDisconnect);
                break;
			case ERROR_IO_PENDING: 	///���������û���ַ��ɶ�
				{ 
					// This is a normal return value if there are no bytes
					// to read at the port.
					// Do nothing and continue
					break;
				}
			case 87:///ϵͳ����
				{
					// Under Windows NT, this value is returned for some reason.
					// I have not investigated why, but it is also a valid reply
					// Also do nothing and continue.
					break;
				}
			default:///�����������������д��ڶ�д�жϿ��������ӵĴ���
				{
					// All other error codes indicate a serious error has
					// occured.  Process this error.
					port->ProcessErrorMessage("WaitCommEvent()");   //�����豸���ڹҵ� ��ʾ��Ϣ ���ڹر���Ϣ
					break;
				}
			}
		}
		else	///WaitCommEvent()����ȷ����
		{
			// If WaitCommEvent() returns TRUE, check to be sure there are
			// actually bytes in the buffer to read.  
			//
			// If you are reading more than one byte at a time from the buffer 
			// (which this program does not do) you will have the situation occur 
			// where the first byte to arrive will cause the WaitForMultipleObjects() 
			// function to stop waiting.  The WaitForMultipleObjects() function 
			// resets the event handle in m_OverlappedStruct.hEvent to the non-signelead state
			// as it returns.  
			//
			// If in the time between the reset of this event and the call to 
			// ReadFile() more bytes arrive, the m_OverlappedStruct.hEvent handle will be set again
			// to the signeled state. When the call to ReadFile() occurs, it will 
			// read all of the bytes from the buffer, and the program will
			// loop back around to WaitCommEvent().
			// 
			// At this point you will be in the situation where m_OverlappedStruct.hEvent is set,
			// but there are no bytes available to read.  If you proceed and call
			// ReadFile(), it will return immediatly due to the async port setup, but
			// GetOverlappedResults() will not return until the next character arrives.
			//
			// It is not desirable for the GetOverlappedResults() function to be in 
			// this state.  The thread shutdown event (event 0) and the WriteFile()
			// event (Event2) will not work if the thread is blocked by GetOverlappedResults().
			//
			// The solution to this is to check the buffer with a call to ClearCommError().
			// This call will reset the event handle, and if there are no bytes to read
			// we can loop back through WaitCommEvent() again, then proceed.
			// If there are really bytes to read, do nothing and proceed.
		
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);

			if (comstat.cbInQue == 0)
				continue;
		}	// end if bResult

		///���ȴ��������������߳�
		// Main wait function.  This function will normally block the thread
		// until one of nine events occur that require action.
		///�ȴ�3���¼����ض�/��/д����һ���¼������ͷ���
		Event = WaitForMultipleObjects(3, ///3���¼�
			port->m_hEventArray, ///�¼�����
			FALSE, ///��һ���¼������ͷ���
			INFINITE);///��ʱʱ��

		switch (Event)
		{
		case 0:
			{
				// Shutdown event.  This is event zero so it will be
				// the higest priority and be serviced first.
				///�ض��¼����رմ���
				CloseHandle(port->m_hComm);
				port->m_hComm=NULL;
				port->m_bThreadAlive = FALSE;
				
				// Kill this thread.  break is not needed, but makes me feel better.
				AfxEndThread(100);

				break;
			}
		case 1:	/// read event������ĸ�����Ϣ���ͳ�ȥ
			{
				memset(&comstat, 0, sizeof(COMSTAT)); 
				GetCommMask(port->m_hComm, &CommEvent);
				if (CommEvent & EV_RXCHAR)//���յ��ַ������������뻺������
				{
					//CEditLog::GetInstance()->Output("-------- read event occur \r\n");
					// Receive character event from port.
					//CEditLog::GetInstance()->Output("------------READ EVENT OCCUR!\r\n");
					ReceiveChar(port, comstat);
				}
				/*if (CommEvent & EV_CTS)//CTS�ź�״̬�����仯
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_CTS_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_BREAK)//�����з����ж�				
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_BREAK_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				if (CommEvent & EV_ERR)//������·״̬������·״̬�������CE_FRAME,CE_OVERRUN��CE_RXPARITY 					
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_ERR_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);				
				if (CommEvent & EV_RING)//��⵽����ָʾ					
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RING_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);				
				if (CommEvent & EV_RXFLAG)//���յ��¼��ַ������������뻺������					
					::SendMessage(port->m_pOwner->m_hWnd, WM_COMM_RXFLAG_DETECTED, (WPARAM) 0, (LPARAM) port->m_nPortNr);
				*/
				break;
			}  
		case 2: /// write event��������
			{
				// Write character event from port
				WriteChar(port);
				break;
			}
		default:
			{
				AfxMessageBox("����������!");
				break;
			}

		} // end switch

	} // close forever loop

	return 0;
}

//
// start comm watching
///���������߳�
//
BOOL CSerialPort::StartMonitoring()
{
	if (!(m_Thread = AfxBeginThread(CommThread, this)))
		return FALSE;
	TRACE("Thread started\n");
	m_bIsSuspened = false;
	//SetThreadAffinityMask(m_Thread->m_hThread,);
	return TRUE;	
}

//
// Restart the comm thread
///��λ�����߳�
//
BOOL CSerialPort::RestartMonitoring()
{
	TRACE("Thread resumed\n");
	m_bIsSuspened = false;
	m_Thread->ResumeThread();
	return TRUE;	
}


//
// Suspend the comm thread
///��������߳�
//
BOOL CSerialPort::StopMonitoring()
{
	TRACE("Thread suspended\n");
	m_bIsSuspened = true;
	m_Thread->SuspendThread(); 
	return TRUE;	
}

void  CSerialPort::SetCallback(SERIAL_CALLBACK_FUN func, void *context)
{
    m_OnCallBackFunc = func;
    m_pContext = context;
}

//
// If there is a error, give the right message
///����д��󣬸�����ʾ
//
void CSerialPort::ProcessErrorMessage(char* ErrorText)
{
	char *Temp = new char[200];
	
	LPVOID lpMsgBuf;

	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		GetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);

	sprintf(Temp, "WARNING:  %s Failed with the following error: \n%s\nPort: %d\n", (char*)ErrorText, lpMsgBuf, m_nPortNr); 
	MessageBox(NULL, Temp, "Application Error", MB_ICONSTOP);

	LocalFree(lpMsgBuf);
	delete [] Temp;
	return;
}

//
// Write a character.
///д����
//
void CSerialPort::WriteChar(CSerialPort* port)
{
	BOOL bWrite = TRUE;
	BOOL bResult = TRUE;

	DWORD BytesSent = 0;

	ResetEvent(port->m_hWriteEvent);///��λд�¼����

	// Gain ownership of the critical section
	EnterCriticalSection(&port->m_csCommunicationSync);

	if (bWrite)
	{
		// Initailize variables
		port->m_ov.Offset = 0;
		port->m_ov.OffsetHigh = 0;

		// Clear buffer
		PurgeComm(port->m_hComm, PURGE_RXCLEAR | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_TXABORT);

		///����д��
		bResult = WriteFile(port->m_hComm,							// Handle to COMM Port
							port->m_szWriteBuffer,					// Pointer to message buffer in calling finction
//							strlen((char*)port->m_szWriteBuffer),	// Length of message to send
							port->m_nWriteSize,	// Length of message to send // add by mrlong
							&BytesSent,								// Where to store the number of bytes sent
							&port->m_ov);							// Overlapped structure

		// deal with any error codes
		if (!bResult)  
		{
			DWORD dwError = GetLastError();
			switch (dwError)
            {
                case ERROR_INVALID_HANDLE:
                    break;
                case ERROR_BAD_COMMAND:
                    SetEvent(port->m_hShutdownEvent);
                    //SendError(ePortDisconnect);
                    break;
				case ERROR_IO_PENDING:
					{
						// continue to GetOverlappedResults()
						BytesSent = 0;
						bWrite = FALSE;
						break;
					}
				default:
					{
						// all other error codes
						//port->ProcessErrorMessage("WriteFile()");    
						break;///
					}
			}
		} 
		else
		{
			LeaveCriticalSection(&port->m_csCommunicationSync);
		}
	} // end if(bWrite)

	if (!bWrite)
	{
		bWrite = TRUE;
	
		bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
									  &port->m_ov,		// Overlapped structure
									  &BytesSent,		// Stores number of bytes sent
									  TRUE); 			// Wait flag

		LeaveCriticalSection(&port->m_csCommunicationSync);

		// deal with the error code 
		//if (!bResult)  ///ע�͵��ˣ�Ϊʲô��
		{
			//port->ProcessErrorMessage("GetOverlappedResults() in WriteFile()");
		}	
	} // end if (!bWrite)

	//Verify that the data size send equals what we tried to send
	if (BytesSent != port->m_nWriteSize)	// Length of message to send)
	{
		TRACE("WARNING: WriteFile() error.. Bytes Sent: %d; Message Length: %d\n", BytesSent, strlen((char*)port->m_szWriteBuffer));
	}
//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED, (WPARAM) RXBuff, (LPARAM) port->m_nPortNr);
//	::SendMessage((port->m_pOwner)->m_hWnd, WM_COMM_TXEMPTY_DETECTED,0,(LPARAM) port->m_nPortNr);
}

//
// Character received. Inform the owner
///������
//
void CSerialPort::ReceiveChar(CSerialPort* port, COMSTAT comstat)
{
	BOOL  bRead = TRUE; 
	BOOL  bResult = TRUE;
	DWORD dwError = 0;
	DWORD BytesRead = 0;
	
	char Buff[2048] = {0};
	char RXBuff[2048] = {0};

	int rcLen = 0;
	int RXLen = 0;

	for (;;) 
	{ 
		if(WaitForSingleObject(port->m_hShutdownEvent,0) == WAIT_OBJECT_0)
            return;

		// Gain ownership of the comm port critical section.
		// This process guarantees no other part of this program 
		// is using the port object. 
		
		//EnterCriticalSection(&port->m_csCommunicationSync);

		// ClearCommError() will update the COMSTAT structure and
		// clear any other errors.
		///����COMSTAT
		
		bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
		
		//LeaveCriticalSection(&port->m_csCommunicationSync);
		
						
		//EnterCriticalSection(&port->m_csCommunicationSync);

		//if (bRead)
		while(comstat.cbInQue != 0 && bResult)
		{
			//rcLen = comstat.cbInQue;
			//BytesRead   = min(rcLen,1024);
			//BytesRead   = min(rcLen,23);
			BytesRead = 1;
			//���ڶ������������������ֽ�
			if(ReadFile(port->m_hComm,		// Handle to COMM port 
							   Buff,				// RX Buffer Pointer
							   BytesRead,			// Read one byte
							   &BytesRead,			// Stores number of bytes read
							   &port->m_ov)		// pointer to the m_ov structure	
							   )
			
			{
				memcpy(RXBuff + RXLen, Buff, BytesRead);
				RXLen += BytesRead;
				memset(Buff, 0, sizeof(Buff));
			}
			else
			{
				bRead = TRUE;
				bResult = GetOverlappedResult(port->m_hComm,	// Handle to COMM port 
											  &port->m_ov,		// Overlapped structure
											  &BytesRead,		// Stores number of bytes read
											  TRUE); 			// Wait flag

				// deal with the error code 
				if (!bResult)  
				{
					port->ProcessErrorMessage("GetOverlappedResults() in ReadFile()");
				}	
			}
			bResult = ClearCommError(port->m_hComm, &dwError, &comstat);
		}
		if(RXLen)
		{
			//ADD CALLBACK FUNCTION
			port->m_OnCallBackFunc(port->m_pContext, RXBuff, RXLen, port->m_nPortNr);

			memset(RXBuff, 0, sizeof(RXBuff));
			RXLen = 0;
		}
		break;
		
	} // end forever loop
}

//
// Write a string to the port
//
void CSerialPort::WriteToPort(char* string)
{		
	assert(m_hComm != 0);
	
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy(m_szWriteBuffer, string);
	m_nWriteSize=strlen(string);

	// set event for write
	SetEvent(m_hWriteEvent);
}


void CSerialPort::WriteToPort(char* string,int n)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));

	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize=n;

	// set event for write
	SetEvent(m_hWriteEvent);
}

void CSerialPort::WriteToPort(LPCTSTR string,int n)
{		
	assert(m_hComm != 0);

	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	memcpy(m_szWriteBuffer, string, n);
	m_nWriteSize = n;

	// set event for write
	SetEvent(m_hWriteEvent);
}

void CSerialPort::WriteToPort(LPCTSTR string)
{		
	assert(m_hComm != 0);
	
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	strcpy(m_szWriteBuffer, string);
	m_nWriteSize=strlen(string);
	
	// set event for write
	SetEvent(m_hWriteEvent);
}

void CSerialPort::WriteToPort(BYTE* Buffer, int n)
{
	assert(m_hComm != 0);
	memset(m_szWriteBuffer, 0, sizeof(m_szWriteBuffer));
	int i;
	for(i=0; i<n; i++)
	{
		m_szWriteBuffer[i] = Buffer[i];
	}
	m_nWriteSize=n;
	
	// set event for write
	SetEvent(m_hWriteEvent);
}

//
// Return the device control block
//
DCB CSerialPort::GetDCB()
{
	return m_dcb;
}

//
// Return the communication event masks
//
DWORD CSerialPort::GetCommEvents()
{
	return m_dwCommEvents;
}

//
// Return the output buffer size
//
DWORD CSerialPort::GetWriteBufferSize()
{
	return m_nWriteBufferSize;
}


void CSerialPort::ClosePort()
{
    if(m_bIsSuspened)
	{
		RestartMonitoring();
	}
    if (m_bThreadAlive)
    {
       // MSG message;
        while (m_bThreadAlive)
        {
			//add by liquanhai  ��ֹ����  2011-11-06
          /*  if(::PeekMessage(&message, m_pOwner->m_hWnd, 0, 0, PM_REMOVE))
            {
                ::TranslateMessage(&message);
                ::DispatchMessage(&message);
            }*/
            SetEvent(m_hShutdownEvent);
        }
        TRACE("Thread ended\n");

    }
    if(m_szWriteBuffer != NULL)
    {
        delete [] m_szWriteBuffer;
        m_szWriteBuffer = NULL;
    }

    if(m_hComm)
    {
        CloseHandle(m_hComm);
        m_hComm = NULL;
    }

	// Close Handles  
	if(m_hShutdownEvent!=NULL)
		ResetEvent(m_hShutdownEvent);
	if(m_ov.hEvent!=NULL)
		ResetEvent(m_ov.hEvent);
	if(m_hWriteEvent!=NULL)
		ResetEvent(m_hWriteEvent);
}

void CSerialPort::SendData(LPCTSTR lpszData, const int nLength)
{
	assert(m_hComm != 0);
	memset(m_szWriteBuffer, 0, nLength);
	strcpy(m_szWriteBuffer, lpszData);
	m_nWriteSize=nLength;
	// set event for write
	SetEvent(m_hWriteEvent);
}

BOOL CSerialPort::RecvData(LPTSTR lpszData, const int nSize)
{
    //
    //��������
    //
    assert(m_hComm!=0);
    memset(lpszData,0,nSize);
    DWORD mylen  = 0;
    DWORD mylen2 = 0;
    while (mylen < (DWORD)nSize) 
	{
        if(!ReadFile(m_hComm,lpszData,nSize,&mylen2,NULL)) 
		{
            return FALSE;
		}
        mylen += mylen2;   
    }
    
    return TRUE;
}

COMMTIMEOUTS CSerialPort:: GetCommTimeOuts()
{
	return m_CommTimeouts;
}

BOOL CSerialPort::SetCommTimeOuts(COMMTIMEOUTS *lpTimeOuts)
{
    SetCommTimeouts(m_hComm, lpTimeOuts);//���ó�ʱ 
	return true;
}


//
///��ѯע���Ĵ��ںţ���ֵ����������
///������ο���mingojiang�Ļ�ȡ�����߼�������
//
void CSerialPort::QueryKey(HKEY hKey) 
{ 
	#define MAX_KEY_LENGTH 255
	#define MAX_VALUE_NAME 16383
	//	TCHAR    achKey[MAX_KEY_LENGTH];   // buffer for subkey name
	//	DWORD    cbName;                   // size of name string 
	TCHAR    achClass[MAX_PATH] = TEXT("");  // buffer for class name 
	DWORD    cchClassName = MAX_PATH;  // size of class string 
	DWORD    cSubKeys=0;               // number of subkeys 
	DWORD    cbMaxSubKey;              // longest subkey size 
	DWORD    cchMaxClass;              // longest class string 
	DWORD    cValues;              // number of values for key 
	DWORD    cchMaxValue;          // longest value name 
	DWORD    cbMaxValueData;       // longest value data 
	DWORD    cbSecurityDescriptor; // size of security descriptor 
	FILETIME ftLastWriteTime;      // last write time 
	
	DWORD i, retCode; 
	
	TCHAR  achValue[MAX_VALUE_NAME]; 
	DWORD cchValue = MAX_VALUE_NAME; 
	
	// Get the class name and the value count. 
	retCode = RegQueryInfoKey(
		hKey,                    // key handle 
		achClass,                // buffer for class name 
		&cchClassName,           // size of class string 
		NULL,                    // reserved 
		&cSubKeys,               // number of subkeys 
		&cbMaxSubKey,            // longest subkey size 
		&cchMaxClass,            // longest class string 
		&cValues,                // number of values for this key 
		&cchMaxValue,            // longest value name 
		&cbMaxValueData,         // longest value data 
		&cbSecurityDescriptor,   // security descriptor 
		&ftLastWriteTime);       // last write time 
	
	for (i=0;i<20;i++)///��Ŵ��ںŵ������ʼ��
	{
		m_nComArray[i] = -1;
	}
	
	// Enumerate the key values. 
	if (cValues > 0) {
		for (i=0, retCode=ERROR_SUCCESS; i<cValues; i++) { 
			cchValue = MAX_VALUE_NAME;  achValue[0] = '\0'; 
			if (ERROR_SUCCESS == RegEnumValue(hKey, i, achValue, &cchValue, NULL, NULL, NULL, NULL))  { 
				CString szName(achValue);
				if (-1 != szName.Find(_T("Serial")) || -1 != szName.Find(_T("VCom")) ){
					BYTE strDSName[10]; memset(strDSName, 0, 10);
					DWORD nValueType = 0, nBuffLen = 10;
					if (ERROR_SUCCESS == RegQueryValueEx(hKey, (LPCTSTR)achValue, NULL, &nValueType, strDSName, &nBuffLen)){
						int nIndex = -1;
						while(++nIndex < MaxSerialPortNum){
							if (-1 == m_nComArray[nIndex]) {
								m_nComArray[nIndex] = atoi((char*)(strDSName + 3));
								break;
							}
						}
					}
				}
			} 
		}
	}
	else{
		AfxMessageBox(_T("����û�д���....."));
	}
	
}

void CSerialPort::Hkey2ComboBox(CComboBox& m_PortNO)
{
	HKEY hTestKey;
	bool Flag = FALSE;
	
	///����XPϵͳ��ע���λ�ã�����ϵͳ����ʵ��������޸�
	if(ERROR_SUCCESS == RegOpenKeyEx(HKEY_LOCAL_MACHINE, TEXT("HARDWARE\\DEVICEMAP\\SERIALCOMM"), 0, KEY_READ, &hTestKey) ){
		QueryKey(hTestKey);
	}
	RegCloseKey(hTestKey);
	
	int i = 0;
	m_PortNO.ResetContent();///ˢ��ʱ����������б�����
	while(i < MaxSerialPortNum && -1 != m_nComArray[i]){
		CString szCom; 
		szCom.Format(_T("COM%d"), m_nComArray[i]);
		m_PortNO.InsertString(i, szCom.GetBuffer(5));
		++i;
		Flag = TRUE;
		if (Flag)///�ѵ�һ�����ֵĴ�����Ϊ�����б��Ĭ��ֵ
			m_PortNO.SetCurSel(0);
	}
	
}

void /*DWORD*/ CSerialPort::SetAffinity(DWORD CPUNo)
{
	//DWORD ret = SetThreadIdealProcessor(m_Thread->m_hThread, CPUNo);
	/*DWORD ret = */SetThreadAffinityMask(m_Thread->m_hThread, CPUNo);
	return /*ret*/;
}

BOOL CSerialPort::SetPriority(int tPriority)
{
	BOOL ret = SetThreadPriority(m_Thread->m_hThread, tPriority);
	return ret;
}