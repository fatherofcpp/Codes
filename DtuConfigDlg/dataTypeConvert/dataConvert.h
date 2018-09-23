#pragma once
/*
数据转换
*/
#include "DataTypeDef.h"
#include "AesConvert.h"

#include "vector"
using namespace std;

extern int gHexType;

//字符串转char,int_8,int_16,int_32,int_64
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

//常规类型转string
template <typename T>
string ConvertToString(T val)
{
	string str;
	stringstream ss;
	ss << val;
	ss >> str;
	return str;
}

//16进制格式字符串转float
void str_16hex_to_float(const char*src, float& val);
//10进制BCD格式字符串转float
void str_ten_hex_to_float(const char*src, float& val);

//gb2312转utf8
int gb2312_to_utf8(const char* gb2312, int gb2312_len, char* utf8);
int get_lenth_gb_to_utf8(const char* gb2312, int gb2312_len);
//utf8转gb2312
int utf8_to_gbk(char * gbk,const char* utf, int utf_len);

extern bool gEncrypt;
//自定义加密
void util_crypt_encrypt(unsigned char *buf, int len, unsigned char token);
//自定义解密
void util_crypt_decrypt(unsigned char *buf, int len, unsigned char token);

//对数值各元素求和
MU_INT8 get_element_sum(char* data,int len);

//按参数sig拆分提取字符串
int SplitToString(string src, std::vector<string>& vec_data, char sig);

//本地数据加密
extern bool gbEncryptLocal;
void encry_local_data(unsigned char *buf, int len);
void decry_local_data(unsigned char *buf, int len);

//实时数据状态
CString AnalyzeMeterState(int state);

//阀值告警状态
CString AnalyzeThresholdState(int state);

//提取实时/历史数据
int ExtractWayData(u_w_measure& w_measure,e_dev_type dev_type,char* data);
//组装cvs数据
int ConvertDataToCvs(u_w_measure& w_measure,e_dev_type dev_type,char* cvs_data);

//crc校验,用于xml通讯
MU_INT16 parity_crc16(MU_INT16 crc,MU_INT8 *buf, int len);
//crc校验,用于下行透传通讯
unsigned short ModbusCrc( unsigned char * pucFrame, unsigned short usLen );


//通信包crc校验
MU_INT16 check_crc_data_communicate_data(str_communicate_data& com_data);

//cs和校验
MU_INT8 parity_cs(MU_INT8* buf, int len);

//12 -> 0x12,单字节10进制数据追加0x前缀
MU_INT8 TenHexAdd0x(MU_INT8 mem);

//字符串小写字母转大写
char * strToUpper(char * str);

//获取当前时间
string GetNowTime();

