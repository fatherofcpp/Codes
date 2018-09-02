#pragma once

#include <windows.h>

namespace NSP_STP_CM
{
	HANDLE OpenPort(const char* strPort_, DWORD BaudRate,BYTE parity,  int read_timeout_ms);

	BOOL Write(HANDLE m_portHandle, const unsigned char * chData_, unsigned int nSizeBuffer_, unsigned long &  nLength_, OVERLAPPED *o = NULL);

	BOOL Read(HANDLE m_portHandle, unsigned char * chData_, unsigned int nSizeBuffer_, unsigned long &  nLength_, OVERLAPPED *o = NULL);

	void ClosePort(HANDLE handle);

	struct tagStSerialOpt;
	class CSerial
	{
	public:
		CSerial();
		~CSerial();
	public:
		void SetStSerialOpt(tagStSerialOpt* p_StSerialOpt);
		HANDLE GetHSerial();//获取串口句柄

		int OnSend(char* senBuff, int byte_to_write, int& byte_written);
		int OnReceive(char* recvBuff, int byte_to_read, int& byte_read);
		int OpenSerial();//打开串口
		void CloseSerial();
		int listenSerial();
	private:
		tagStSerialOpt* m_pStSerialOpt;//串口通讯参数
		HANDLE m_hSerial;//串口句柄
	};
}
