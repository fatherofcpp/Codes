#include "stdafx.h"
#include "Uart.h"
#include "xmlAnalyze.h"
#include "log.h"
#include "dataConvert.h"

CSerialAPI::CSerialAPI()
{
	pSerial = new st_serial_conf;
	memset(pSerial, 0, sizeof(*pSerial));
	m_hIDComDev = INVALID_HANDLE_VALUE;

}

int CSerialAPI::OpenUart()
{
	CString str;

	DCB com_dcb; //设备控制块

	m_hIDComDev = CreateFile (pSerial->com_name, GENERIC_READ| GENERIC_WRITE,0,

		NULL,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);

	if (m_hIDComDev == INVALID_HANDLE_VALUE)
	{
		str.Format("无法打开串行口%s",pSerial->com_name);
		AfxMessageBox(str);
		return -1;
	}
	else
	{
		COMMTIMEOUTS comm_time_outs ;
		SetCommMask(m_hIDComDev, EV_RXCHAR ) ;
		SetupComm(m_hIDComDev, pSerial->buff_size, pSerial->buff_size) ; /*4096设置收发缓冲区 尺寸为4K */
		PurgeComm(m_hIDComDev, PURGE_TXABORT| PURGE_RXABORT |
			PURGE_TXCLEAR| PURGE_RXCLEAR ) ; //清收发缓冲区

		//以下初始化结构变量CommTimeOuts, 设置超时参数 CommTimeOuts.ReadIntervalTimeout = 0xFFFFFFFF ;
		comm_time_outs.ReadIntervalTimeout = 0;
		comm_time_outs.ReadTotalTimeoutMultiplier = 0 ;
		comm_time_outs.ReadTotalTimeoutConstant = pSerial->read_timeout_m;//4000
		comm_time_outs.WriteTotalTimeoutMultiplier = 0;
		comm_time_outs.WriteTotalTimeoutConstant = pSerial->write_timeout_m ;//4000
		SetCommTimeouts(m_hIDComDev, &comm_time_outs ); //设置超时参数

		com_dcb.DCBlength = sizeof( com_dcb ) ;
		GetCommState( m_hIDComDev, &com_dcb ) ; //获取当前参数
		com_dcb.BaudRate = pSerial->baud; //波特率
		com_dcb.ByteSize = pSerial->byte_size; //8数据位
		com_dcb.Parity = pSerial->parity; /*0校验 0~4=no, odd, even, mark, space */
		SetCommState(m_hIDComDev, &com_dcb ) ;//设置通讯参数
		return 0;
	} //设置新的通信参数
}

int CSerialAPI::OnSend(char* senBuff, int len)
{
	DWORD dwBytesWritten;
	int nWriteLen = 1;
	int tmpLen = 0;

	for(int i = 0; i < len; i++)
	{
		WriteFile(m_hIDComDev, (LPSTR)(senBuff + tmpLen), nWriteLen, &dwBytesWritten, NULL);
		if(dwBytesWritten <= 0)
		{
			return -1;
		}
		tmpLen += dwBytesWritten;
	}
	return 0;
}

//1.2 接收例程
int CSerialAPI::OnReceive(char* recvBuff, int len)
{
	DWORD dRead,dReadNum;
	int nInd = 0;
	int nReadLen = 1;
	int tmpLen = 0;
	char* pKeyBuf = NULL;

	while(1)
	{
		dRead=ReadFile(m_hIDComDev, (LPSTR)(recvBuff + tmpLen), nReadLen, &dReadNum, NULL); 
		if(dReadNum <= 0)
		{
			return -1;	
		}
		tmpLen += dReadNum;

		if(tmpLen == len)
		{
			return 0;
		}
	}
}

CSerialAPI::~CSerialAPI()
{
	delete pSerial;
	if (m_hIDComDev != NULL)
	{
		CloseHandle(m_hIDComDev);
	}
}

HANDLE CSerialAPI::GetIDComDev()
{
	return m_hIDComDev;
}

void CSerialAPI::CloseUart()
{
	if (m_hIDComDev != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hIDComDev);
		m_hIDComDev = INVALID_HANDLE_VALUE;
	}
}

void CSerialAPI::SetSerialConf( st_serial_conf*serial )
{
	memcpy(pSerial, serial, sizeof(*pSerial));
}



