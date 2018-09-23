#pragma once

#define FC_DTRDSR       0x01
#define FC_RTSCTS       0x02
#define FC_XONXOFF      0x04
#define ASCII_BEL       0x07
#define ASCII_BS        0x08
#define ASCII_LF        0x0A
#define ASCII_CR        0x0D
#define ASCII_XON       0x11
#define ASCII_XOFF      0x13

typedef struct
{
	char com_name[32];//COM4
	int baud;//波特率9600
	int byte_size;//数据位
	int parity;//校验 0~4=no, odd, even, mark, space
	int read_timeout_m;//读超时参数4000
	int write_timeout_m;//写超时参数4000
	int buff_size;//收发缓冲区
}st_serial_conf;

class CSerialAPI 
{
public:
	CSerialAPI();
	~CSerialAPI();

public:
	int OnSend(char* senBuff, int len);
	int OnReceive(char* recvBuff, int len);
	void SetSerialConf(st_serial_conf*serial);
	int OpenUart();//打开串口
	void CloseUart();
	HANDLE GetIDComDev();//获取串口句柄
private:
	st_serial_conf* pSerial;//串口通讯参数
	HANDLE m_hIDComDev;//串口句柄
};
