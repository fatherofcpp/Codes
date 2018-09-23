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
	int baud;//������9600
	int byte_size;//����λ
	int parity;//У�� 0~4=no, odd, even, mark, space
	int read_timeout_m;//����ʱ����4000
	int write_timeout_m;//д��ʱ����4000
	int buff_size;//�շ�������
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
	int OpenUart();//�򿪴���
	void CloseUart();
	HANDLE GetIDComDev();//��ȡ���ھ��
private:
	st_serial_conf* pSerial;//����ͨѶ����
	HANDLE m_hIDComDev;//���ھ��
};
