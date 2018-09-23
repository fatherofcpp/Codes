#include "stdafx.h"
#include "MOXASerialport.h"

MoxaSerialport::MoxaSerialport(int com, int bd):m_len(12)
{
	spcom = com;
	spbd = bd;
	hExit = NULL;
	hThread = NULL;
}

MoxaSerialport::~MoxaSerialport()
{}

int MoxaSerialport::Open()
{
	int Baudrate = BDConvert(spbd);
	/* port =2 means ˉCOM2ˇ */

	/* Port Control, enable the port */
	int ret = sio_open (spcom);

	if (ret == SIO_OK) 
	{

		/* Port Control, set baud, parity... */
		sio_ioctl (spcom, Baudrate, P_NONE | BIT_8 | STOP_1 );

		/* Output Data function */
		//sio_write (CommPort, "ABCDE", 5);

		/* Input Data function */
		//sio_read (port, ibuf, length);

		if((hExit = CreateEvent(NULL,TRUE,FALSE,NULL))==NULL)
            return 1;
		return 0;
	}
	else
	{
		return 1;
	}
}

int MoxaSerialport::Close()
{
	CloseHandle(hExit);
	int ret = sio_close (spcom);
	if(ret == SIO_OK)
		return 0;
	else
		return 1;
}

int  MoxaSerialport::Send(char* buff, int len)
{
	int ret = sio_write(spcom, buff, len);
	return ret;
}
void MoxaSerialport::Start()
{
	ResetEvent(hExit);
	hThread = (HANDLE)_beginthread(Recvthread, 0, this);
}
void MoxaSerialport::Stop()
{
	SetEvent(hExit);
	
	if(hThread)
		CloseHandle(hThread);
}
void MoxaSerialport::SetAffinity(DWORD CPUNo)
{
	SetThreadAffinityMask(hThread, CPUNo);
}

void MoxaSerialport::SetCallBack(DataReadCallBack callback, void* pContext)
{
	m_Callback = callback;
	m_pContext = pContext;
}

void MoxaSerialport::Recvthread(LPVOID param)
{
	MoxaSerialport* dlg = (MoxaSerialport*)param;
	DWORD	dRes;
	int	len;
	char buf[512] = {0}; 
	CString str;
	static LONGLONG oldqp = 0;

	while(1)
	{
        dRes = WaitForSingleObject(dlg->hExit,0);
        switch(dRes)
		{
        case WAIT_OBJECT_0:/* got hExit event,exit thread. */
            return ;
        default:
            break;
        }

		len = sio_read(dlg->spcom,buf,dlg->m_len);
        if(len>0)
		{
			dlg->m_Callback(dlg->m_pContext, buf, len);
		}
	}
}

int MoxaSerialport::Read(string& str)
{
	char buf[1024] = {0};
	int len = sio_read(spcom,buf,12);
	str.assign(buf, len);
	return len;
}

//波特率转换函数：从数字波特率转换成MOXA定义的波特率
int MoxaSerialport::BDConvert(int bd)
{
	switch(bd)
	{
	case 50:
		return 0x00;
	case 75:
		return 0x01;
	case 110:
		return 0x02;
	case 134:
		return 0x03;
	case 150:
		return 0x04;
	case 300:
		return 0x05;
	case 600:
		return 0x06;
	case 1200:
		return 0x07;
	case 1800:
		return 0x08;
	case 2400:
		return 0x09;
	case 4800:
		return 0x0A;
	case 7200:
		return 0x0B;
	case 9600:
		return 0x0C;
	case 19200:
		return 0x0D;
	case 38400:
		return 0x0E;
	case 57600:
		return 0x0F;
	case 115200:
		return 0x10;
	case 230400:
		return 0x11;
	case 460800:
		return 0x12;
	case 921600:
		return 0x13;
	}
	return -1;
}