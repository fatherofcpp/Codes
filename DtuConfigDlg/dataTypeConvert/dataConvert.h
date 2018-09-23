#pragma once
/*
����ת��
*/
#include "DataTypeDef.h"
#include "AesConvert.h"

#include "vector"
using namespace std;

extern int gHexType;

//�ַ���תchar,int_8,int_16,int_32,int_64
template <typename T>
void str_to_int(const char*src, T &val, int type = 10)
{
	int size_val = sizeof(T);
	int tmp_val = 0;

	switch(type)
	{
	case 10:
		if (size_val == 1)
		{
			sscanf(src,"%d",&tmp_val);
			val = (T)(tmp_val & 0xFF);
		}
		else if (size_val == 2)
		{
			sscanf(src,"%d",&tmp_val);
			val = (T)(tmp_val & 0xFFFF);
		}
		else if (size_val == 4)
		{
			sscanf(src,"%d",&val);
		}
		else if (size_val == 8)
		{
			sscanf(src,"%lld",&val);
		}
		else
		{
			assert(0);
		}
		break;
	case 16:
		sscanf(src,"%x",&val);
		break;
	default:
		break;
	}
}

//��������תstring
template <typename T>
string ConvertToString(T val)
{
	string str;
	stringstream ss;
	ss << val;
	ss >> str;
	return str;
}

//16���Ƹ�ʽ�ַ���תfloat
void str_16hex_to_float(const char*src, float& val);
//10����BCD��ʽ�ַ���תfloat
void str_ten_hex_to_float(const char*src, float& val);

//gb2312תutf8
int gb2312_to_utf8(const char* gb2312, int gb2312_len, char* utf8);
int get_lenth_gb_to_utf8(const char* gb2312, int gb2312_len);
//utf8תgb2312
int utf8_to_gbk(char * gbk,const char* utf, int utf_len);

extern bool gEncrypt;
//�Զ������
void util_crypt_encrypt(unsigned char *buf, int len, unsigned char token);
//�Զ������
void util_crypt_decrypt(unsigned char *buf, int len, unsigned char token);

//����ֵ��Ԫ�����
MU_INT8 get_element_sum(char* data,int len);

//������sig�����ȡ�ַ���
int SplitToString(string src, std::vector<string>& vec_data, char sig);

//�������ݼ���
extern bool gbEncryptLocal;
void encry_local_data(unsigned char *buf, int len);
void decry_local_data(unsigned char *buf, int len);

//ʵʱ����״̬
CString AnalyzeMeterState(int state);

//��ֵ�澯״̬
CString AnalyzeThresholdState(int state);

//��ȡʵʱ/��ʷ����
int ExtractWayData(u_w_measure& w_measure,e_dev_type dev_type,char* data);
//��װcvs����
int ConvertDataToCvs(u_w_measure& w_measure,e_dev_type dev_type,char* cvs_data);

//crcУ��,����xmlͨѶ
MU_INT16 parity_crc16(MU_INT16 crc,MU_INT8 *buf, int len);
//crcУ��,��������͸��ͨѶ
unsigned short ModbusCrc( unsigned char * pucFrame, unsigned short usLen );


//ͨ�Ű�crcУ��
MU_INT16 check_crc_data_communicate_data(str_communicate_data& com_data);

//cs��У��
MU_INT8 parity_cs(MU_INT8* buf, int len);

//12 -> 0x12,���ֽ�10��������׷��0xǰ׺
MU_INT8 TenHexAdd0x(MU_INT8 mem);

//�ַ���Сд��ĸת��д
char * strToUpper(char * str);

//��ȡ��ǰʱ��
string GetNowTime();

