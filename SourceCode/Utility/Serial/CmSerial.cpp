#include "stdafx.h"
#include "CmSerial.h"
#include "CmFunction/CmDefStruct.h"
#include <stdio.h>

HANDLE NSP_STP_CM::OpenPort(const char* strPort_, DWORD BaudRate, BYTE parity, int read_timeout_ms)
{
	USES_CONVERSION;

	HANDLE m_portHandle = NULL;
	DCB m_portConfig;
	COMMTIMEOUTS comTimeOut;
	m_portHandle = CreateFile(A2W(strPort_),		// Specify port device: default "COM1"
		GENERIC_READ | GENERIC_WRITE,       // Specify mode that open device.
		0,                                  // the devide isn't shared.
		NULL,                               // the object gets a default security.
		OPEN_EXISTING,                      // Specify which action to take on file. 
		0,/*FILE_ATTRIBUTE_NORMAL|FILE_FLAG_OVERLAPPED*/ //异步方式打开
		NULL);                              // default.

	if (NULL == m_portHandle || INVALID_HANDLE_VALUE == m_portHandle)
	{
		return NULL;
	}

	SetupComm(m_portHandle,1024,1024); //设置发送和接收缓冲区大小

	// Get current configuration of serial communication port.
	if (GetCommState(m_portHandle,&m_portConfig) == 0)
	{
		printf("can not opened com %s\n", strPort_);
		return m_portHandle;
	}

	//sSet current configuration of serial communication port.
	m_portConfig.ByteSize = 8;            // Byte of the Data.
	m_portConfig.StopBits = ONESTOPBIT;   // Use one bit for stopbit.
	m_portConfig.Parity = parity;       // No parity bit
	m_portConfig.BaudRate = BaudRate;    // Buadrate 9600 bit/sec
	m_portConfig.fBinary = TRUE;
	m_portConfig.fParity = TRUE;

	if (SetCommState(m_portHandle,&m_portConfig) == 0)
	{
		printf("can not opened com %s\n", strPort_);
		return m_portHandle;
	}

	if (!SetCommMask(m_portHandle,EV_RXCHAR | EV_TXEMPTY))
	{
		printf("SetCommMask error %s\n", strPort_);
		return FALSE;
	}

	// instance an object of COMMTIMEOUTS.
	comTimeOut.ReadIntervalTimeout = 0;
	comTimeOut.ReadTotalTimeoutMultiplier = 0;
	comTimeOut.ReadTotalTimeoutConstant = read_timeout_ms; //读总超时
	comTimeOut.WriteTotalTimeoutMultiplier = 0;
	comTimeOut.WriteTotalTimeoutConstant = 40;

	if (FALSE == SetCommTimeouts(m_portHandle,&comTimeOut))
	{
		printf("can not opened com %s\n", strPort_);
		return FALSE;
	}

	printf("success opened com:%s\n", strPort_);

	return m_portHandle;
}

void NSP_STP_CM::ClosePort(HANDLE handle)
{
	if (INVALID_HANDLE_VALUE != handle)
	{
		CloseHandle(handle);
	}
}

BOOL NSP_STP_CM::Write(HANDLE m_portHandle, const unsigned char * chData_, unsigned int nSizeBuffer_, unsigned long &  nLength_ , OVERLAPPED * /*o*/ )
{
	if(m_portHandle == NULL) return FALSE;
	if (nSizeBuffer_ > 0)
	{
		if (WriteFile(m_portHandle, // handle to file to write to
			chData_,                // pointer to data to write to file
			nSizeBuffer_,           // number of bytes to write
			&nLength_,				// pointer to number of bytes written
			NULL) == 0)      
		{
			return FALSE;
		}
		return TRUE;
	}

	return FALSE;
}

BOOL NSP_STP_CM::Read(HANDLE m_portHandle, unsigned char * chData_, unsigned int nSizeBuffer_, unsigned long&  nLength_, OVERLAPPED * /*o*/)
{
	if (m_portHandle == NULL) return FALSE;
	if (ReadFile(m_portHandle, // handle of file to read
		chData_,               // handle of file to read
		nSizeBuffer_,          // number of bytes to read
		&nLength_,             // pointer to number of bytes read
		NULL) == 0)               // pointer to structure for data
	{
		return FALSE;
	}

	if (nLength_ > 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
	return FALSE;
}

////////////////////////////////////////////////
NSP_STP_CM::CSerial::CSerial()
{
	m_pStSerialOpt = new tagStSerialOpt;
	m_hSerial = INVALID_HANDLE_VALUE;
}

NSP_STP_CM::CSerial::~CSerial()
{
	delete m_pStSerialOpt;
	if (m_hSerial != NULL)
	{
		CloseHandle(m_hSerial);
	}
}

void NSP_STP_CM::CSerial::SetStSerialOpt( tagStSerialOpt* p_StSerialOpt )
{
	*m_pStSerialOpt = *p_StSerialOpt;
}

HANDLE NSP_STP_CM::CSerial::GetHSerial()
{
	return m_hSerial;
}

int NSP_STP_CM::CSerial::OnSend(char* senBuff, int byte_to_write, int& byte_written)
{
	if(m_hSerial == NULL || byte_to_write <= 0) 
	{
		return -1;
	}

	DWORD dw_bytes_written;
	BOOL res = WriteFile(m_hSerial, senBuff, byte_to_write, &dw_bytes_written, NULL);
	byte_written = dw_bytes_written;
	if(res == FALSE || dw_bytes_written != static_cast<DWORD>(byte_to_write))
	{
		return -1;
	}

	return 0;
}

int NSP_STP_CM::CSerial::OnReceive( char* recvBuff, int byte_to_read, int& byte_read )
{
	if(m_hSerial == NULL || byte_to_read <= 0) 
	{
		return -1;
	}

	DWORD read_num;
	BOOL res = ReadFile(m_hSerial, recvBuff, byte_to_read, &read_num, NULL); 
	byte_read = read_num;
	if(res == FALSE || read_num != static_cast<DWORD>(byte_to_read))
	{
		return -1;
	}
	return 0;
}

int NSP_STP_CM::CSerial::OpenSerial()
{
	USES_CONVERSION;

	DCB com_dcb; //设备控制块

	m_hSerial = CreateFile (A2W(m_pStSerialOpt->com_name.c_str()), 
		GENERIC_READ| GENERIC_WRITE,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (m_hSerial == INVALID_HANDLE_VALUE)
	{
		return -1;
	}
	else
	{
		SetCommMask(m_hSerial, EV_RXCHAR ) ;
		SetupComm(m_hSerial, m_pStSerialOpt->buff_size, m_pStSerialOpt->buff_size) ; /*4096设置收发缓冲区 尺寸为4K */
		PurgeComm(m_hSerial, PURGE_TXABORT| PURGE_RXABORT |
			PURGE_TXCLEAR| PURGE_RXCLEAR ) ; //清收发缓冲区

		//以下初始化结构变量CommTimeOuts, 设置超时参数 CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
		COMMTIMEOUTS comm_time_outs ;
		comm_time_outs.ReadIntervalTimeout = 0;
		comm_time_outs.ReadTotalTimeoutMultiplier = 0 ;
		comm_time_outs.ReadTotalTimeoutConstant = m_pStSerialOpt->read_timeout_m;//4000
		comm_time_outs.WriteTotalTimeoutMultiplier = 0;
		comm_time_outs.WriteTotalTimeoutConstant = m_pStSerialOpt->write_timeout_m ;//4000
		SetCommTimeouts(m_hSerial, &comm_time_outs ); //设置超时参数

		com_dcb.DCBlength = sizeof( com_dcb ) ;
		GetCommState( m_hSerial, &com_dcb ) ; //获取当前参数
		com_dcb.BaudRate = m_pStSerialOpt->baud; //波特率
		com_dcb.StopBits = static_cast<BYTE>(m_pStSerialOpt->stop); //停止位
		com_dcb.ByteSize = static_cast<BYTE>(m_pStSerialOpt->byte_size); //8数据位
		com_dcb.Parity = static_cast<BYTE>(m_pStSerialOpt->parity); /*0校验 0~4=no, odd, even, mark, space */
		SetCommState(m_hSerial, &com_dcb ) ;//设置通讯参数
		return 0;
	} //设置新的通信参数
}

void NSP_STP_CM::CSerial::CloseSerial()
{
	if (m_hSerial != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSerial);
		m_hSerial = INVALID_HANDLE_VALUE;
	}
}

int NSP_STP_CM::CSerial::listenSerial()
{
	const int r_t_out_m = m_pStSerialOpt->read_timeout_m;
	const int single_slp_m = 10;
	const int r_count = r_t_out_m / single_slp_m;

	COMSTAT com_stat;
	DWORD dw_error_flags = 0;
	DWORD w_count = 0;
	CString str;
	int data_in_len = -1;

	for (int i = 0; i < r_count; i++)
	{
		Sleep(single_slp_m);
		ClearCommError(m_hSerial, &dw_error_flags, &com_stat);
		w_count = com_stat.cbInQue;
		if(w_count > 0) 
		{
			data_in_len = w_count;
			break;
		}
	}

	return data_in_len;
}
